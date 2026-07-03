// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_SESSION_HPP
#define SEAM_SESSION_HPP

#include "../common/cbasetypes.hpp"   // int32 / uint32

// [SATHENA-SEAM interface] SessionSeam — session / routing policy across the map & char servers.
// keepMap fires in map_addmap (return false to DROP the map from THIS instance = runtime sharding);
// onLogAtcommand taps every atcommand; onCharOnline/onCharOffline (char server) let a consumer own
// a character's online-state tracking out-of-band from the account-keyed online_char_db;
// onSessionKeptAfterDetach (map server) decouples a persistent clientless session's map object id
// from its account_id so more than one session per account can coexist in-world. Defaults
// are no-ops (pure vanilla). A consumer build replaces the instance via session_seam() = &instance;
// from a static registrar or early init. The accessor uses function-local statics so registration
// order is safe. Signatures use only primitives + a forward-declared type so BOTH servers compile it.

class map_session_data;
struct block_list;
struct char_session_data;
struct CHARACTER_INFO;

// Action selector for actionAllowed(). Values name the concrete operation being gated; a
// consumer returns false for any it wishes to forbid for a given session.
enum e_seam_action : int32{
	SEAM_ACTION_CHAR_DELETE  = 1, // deleting a character (char server)
	SEAM_ACTION_STORAGE_OPEN = 2, // opening account storage (map server)
};

struct SessionSeam{
	// Fires in map_addmap for every map in the server's list; return false to DROP the map
	// from THIS instance, so one binary serves any subset per launch (shard assignment).
	virtual bool keepMap( const char* mapname ){ return true; }

	// atcommand invocation. message = the full command text.
	virtual void onLogAtcommand( const map_session_data& sd, const char* message ){}

	// Fires at the TOP of char_set_char_online (char server). Return true to have a consumer take
	// over this character's online-state tracking out-of-band from the account-keyed online_char_db,
	// skipping the vanilla account-slot registration. Default false => vanilla.
	virtual bool onCharOnline( int32 map_id, uint32 char_id, uint32 account_id ){ return false; }

	// Fires at the TOP of char_set_char_offline; pairs with onCharOnline. Return true if a consumer
	// owns this character's offline teardown out-of-band, to skip the vanilla path. Default false.
	virtual bool onCharOffline( uint32 char_id, uint32 account_id ){ return false; }

	// Fires when a session is kept in-world as a persistent background session with NO attached
	// client — either the client just detached, or the session was recovered clientless. A
	// consumer may re-home the session to a distinct map object id (see map_reassign_object_id) so
	// more than one session per account can coexist in-world. Fires AFTER the session is fully
	// loaded, so id-decoupling does not disturb account-keyed load. Default no-op.
	virtual void onSessionKeptAfterDetach( block_list& bl ){}

	// Fires in char_mmo_chars_fromsql after the account's own characters were written to the
	// selectable list, before the packet count is finalized. A consumer may add selectable
	// characters to this account's list: write each into chars[] at the running index, register
	// its id in sd.found_char[<a free slot>], advance `count`, and return the total bytes it
	// appended (added to the packet length). Default 0 => nothing added (vanilla).
	virtual int32 appendSelectableChars( char_session_data& sd, CHARACTER_INFO chars[], int32& count ){ return 0; }

	// Resolves which character a selected slot refers to. default_char_id = the id resolved from
	// the account's own characters (0 if the account's own characters do not cover this slot). A
	// consumer may return the id of another character it made selectable for this slot. Default =
	// pass through the account's own resolution.
	virtual uint32 resolveSelectedChar( char_session_data& sd, int32 slot, uint32 default_char_id ){ return default_char_id; }

	// Returns false to forbid an action for a session. account_id/char_id identify the acting
	// session and the character it acts on; action_id selects the operation (see e_seam_action).
	// Default: every action allowed (vanilla).
	virtual bool actionAllowed( uint32 account_id, uint32 char_id, int32 action_id ){ return true; }

	virtual ~SessionSeam() = default;
};

inline SessionSeam*& session_seam(){
	static SessionSeam s_default;
	static SessionSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_SESSION_HPP */
