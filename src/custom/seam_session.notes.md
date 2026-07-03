# SessionSeam — dev notes (mechanism only)

> Per-seam working notes: **why** the hooks are shaped this way + **gotchas**, so the next change here
> starts informed. Mechanism only — no product/application detail (that lives in the private thenro
> docs, referenced at the bottom). Keep this updated whenever you touch SessionSeam.

## What it is
Session / routing policy spanning the **login, char, and map** servers. One header
(`seam_session.hpp`), compiled into all three; **each server links its own consumer instance** — there
is NO shared object across the server processes. Cross-server state goes in the DB or the char-server
ledger, never in the seam. Single-threaded per process → consumers need no locks.

## Hooks (all default = vanilla no-op)
| hook | server | fires | purpose |
|---|---|---|---|
| `keepMap(mapname)` | map | `map_addmap` per map | return false → drop the map from THIS instance (runtime sharding) |
| `onLogAtcommand(sd,msg)` | map | top of `log_atcommand` | audit tap, before the config gate |
| `onCharOnline(map_id,cid,aid)` | char | top of `char_set_char_online` | return true → consumer tracks this char's online state OUT-OF-BAND (skip the account-keyed `online_char_db` slot) |
| `onCharOffline(cid,aid)` | char | top of `char_set_char_offline` | pairs with onCharOnline; return true → consumer owns teardown |
| `onSessionKeptAfterDetach(bl)` | map | after a client detaches but the session persists | consumer may re-home the session to a distinct map object id (`map_reassign_object_id`) so >1 session/account can coexist in-world |
| `appendSelectableChars(sd,chars,count)` | char | end of `char_mmo_chars_fromsql` | consumer may append extra selectable characters to an account's list (write `chars[count]`, register in `sd.found_char[]`, advance `count`, return bytes) |
| `resolveSelectedChar(sd,slot,default_cid)` | char | `chclif_parse_charselect` char-id resolution | consumer may resolve a selected slot to a character not in the account's own list; default passes the account's own resolution through |
| `actionAllowed(aid,cid,action_id)` | char + map | char-delete at the `char_delete()` CHOKE (+ `chclif_parse_char_delete2_req` for early UX) + storage-open (`storage_storageopen`) | return false to forbid; `action_id` from `e_seam_action` distinguishes call-sites |

Helper (map.cpp, generic): `map_reassign_object_id(bl, new_id)` — re-keys `id_db` + `pc_db` (both keyed
by `bl->id`), leaves `charid_db` (keyed by char_id) alone.

Inline mechanism (chrif.cpp `chrif_authok`, `#ifdef SATHENA`, no hook — self-activating, inert in
vanilla): when the played account (from the character's own record, `status.account_id`) differs from
the login account that authenticated, align the session with the played account BEFORE it is indexed —
set `sd->id` and re-home the `auth_db` node to `status.account_id`. This is required for the
account-keyed load pushes (inventory/cart/storage/regs via `map_id2sd(account_id)`, all arriving after
authok, indexed at `map_addiddb` in `pc_reg_received`) to land, and for auth-finish/save-ack/logout
(all keyed by `status.account_id`) to stay consistent. Vanilla always has the two accounts equal, so
it is a no-op there.

## Gotchas / whys (learned the hard way)
- **Header must forward-declare, not include, map types.** `seam_session.hpp` forward-declares
  `map_session_data` + `block_list` so `char.cpp` (which has neither) compiles. onCharOnline/Offline use
  only primitives for the same reason. Don't `#include` a map header here.
- **`onSessionKeptAfterDetach` fires AFTER full load, NOT at `pc_setnewpc`.** The first design decoupled
  the map id at birth (`sd->id = mapObjectId(...)` in pc_setnewpc) — it BROKE: a boot-recovered clientless
  session loads its state (cart/inventory/status) via account-keyed `map_id2sd(account_id)` pushes, and the
  load handler schedules follow-up work; a decoupled id makes all of that miss. So re-home the id only
  after the session is fully loaded. Fired from both the live-detach path (`ACMD_FUNC(autotrade)`) and the
  boot-recovery path (`pc_autotrade_timer`).
- **`pc_db` is keyed by `sd->id`** (map.cpp:2175), NOT by account_id — so reassigning `sd->id` re-keys
  `id_db` + `pc_db` together and there is no hidden second account-keyed index to collide.
- **`char_set_char_offline(-1, account)` is a MASS clear** — it sets every char of the account offline.
  A consumer tracking a background char out-of-band MUST special-case `cid == -1` to preserve its own
  session, or the background session gets knocked offline racily.
- **Decoupled-id sessions are invisible to account-keyed live delivery.** Everything doing
  `map_id2sd(account_id)` (guild/party/friend-online/mail/intif/chrif) returns null for a re-homed
  session. That's by design; the account is represented by whatever normal session holds the real id.
- **searchstore/buyingstore reference a shop by the vendor's map id** (the id the buyer got in the
  vending/board packet). Store `sd->id` (not `status.account_id`) in the searchstore entry (vending.cpp /
  buyingstore.cpp) so lookups stay consistent when the id is decoupled — identical for normal players.
- **Played-account != login-account decouples opposite to a background vendor.** For an injected
  selectable char, the char-select `auth_db` (char server) is keyed by the CLIENT's login account
  (the client can only send that), so `node->account_id` must stay the login account through the
  map<->char handshake; the played account only enters at `pc_authok`'s memcpy of the character record.
  So the re-home MUST happen at `chrif_authok` (after the memcpy, before `pc_reg_received`) — NOT at
  char-select (breaks the char-server handshake) and NOT at `char_set_char_online` (splits the
  char-server online slot between the two accounts). And unlike a background vendor (synthetic id !=
  its own account → account-keyed load misses → decouple only AFTER load), here `sd->id` is set to the
  played account (== the load account) so the load LANDS — decoupling at authok is correct precisely
  because the two ids agree.
- **`actionAllowed` for char-delete gates the CHOKE, not the request handler.** The real deletion is
  `char_delete()` (char.cpp), reachable from THREE request handlers (delete2 reserve, delete2 accept,
  legacy single-step delchar). Gating only the reserve handler leaves accept + delchar open. Put the gate
  at `char_delete()` itself (returns a refuse code) so one gate covers every path; keep an optional gate
  at the reserve handler for early UX. Same rule for any future forbiddable op: gate where the effect
  happens, not at one of its callers.

## Build / test gotchas
- A single-seam branch has only its own + `ContentSeam` headers, and the full `thenro/src` consumer is
  broken post-rebuild (old accessors). To COMPILE-check the seam inert: `./build.sh --vanilla --test
  <char|map>`. To test WITH a consumer: build against a MINIMAL isolated `CUSTOM_SRC_DIR` (not thenro/src).
- Backgrounded servers do NOT persist across tool calls; server logs must go UNDER the scratchpad (bare
  `/tmp` is sandbox-blocked for bg procs).
- Killing test servers: use `pkill -x <name>` — `pkill -f 'X-server'` matches the tool's own wrapper and
  self-kills the command.

## Product/application context (private)
The consumer applications that fill these hooks (and their whys) are in the PRIVATE thenro docs, not
here: `thenro/docs/SESSIONSEAM-AUTOTRADER-DESIGN.md`, `thenro/docs/AUTOTRADER-NOTES.md`,
`thenro/docs/FEATURE-SEAM-MAP.md`. Never copy that detail into this file.
