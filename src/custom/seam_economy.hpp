// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_ECONOMY_HPP
#define SEAM_ECONOMY_HPP

// [SATHENA-SEAM interface] EconomySeam — the zeny/drop/vending economy family. onZenyChange fires
// after every pc_getzeny/pc_payzeny (signed applied delta); onVendingTax post-adjusts the vending
// rate; onDropRate mutates the per-context drop rate; and the item/zeny/branch/mvp/cash log taps
// observe every economy event at their log_* sinks. Defaults are no-ops => vanilla. A consumer
// build replaces the instance via economy_seam() = &my_instance; the accessor uses function-local
// statics so registration order is safe.

#include "../common/cbasetypes.hpp"   // int16 / int32 / int64 / uint32

class map_session_data;
class block_list;
class mob_data;
struct item;

struct EconomySeam{
	// signed applied zeny delta (+gained / -paid) after pc_getzeny/pc_payzeny, with the log-type.
	virtual void onZenyChange( map_session_data* sd, int32 delta, int32 log_type ){}

	// vending tax — the vanilla-resolved rate (basis points) is mutable by-ref before it applies.
	virtual void onVendingTax( map_session_data* sd, double zeny, double& tax_rate ){}

	// per-context drop-rate decision (mob_getdroprate); drop_rate (0..10000 bp) mutable in place.
	virtual void onDropRate( block_list* killer, mob_data* md, int32& drop_rate ){}

	// item flow. id = char/account id, m = mapid, type = e_log_pick_type, itm carries nameid.
	virtual void onLogPick( int32 id, int16 m, int32 type, int32 amount, const item* itm ){}
	// zeny flow. type = e_log_pick_type, src_id = counterparty id, amount signed.
	virtual void onLogZeny( const map_session_data& sd, int32 type, uint32 src_id, int32 amount ){}
	// summon-item (dead/bloody branch) use.
	virtual void onLogBranch( const map_session_data& sd ){}
	// MVP kill reward. monster_id = mob, nameid = prize item (0 if none), exp = mvp exp.
	virtual void onLogMvpdrop( const map_session_data& sd, int32 monster_id, uint32 nameid, int64 exp ){}
	// cash-point (Kafra/premium) flow. type = e_log_pick_type, cash_type = e_log_cash_type.
	virtual void onLogCash( const map_session_data& sd, int32 type, int32 cash_type, int32 amount ){}

	virtual ~EconomySeam() = default;
};

inline EconomySeam*& economy_seam(){
	static EconomySeam s_default;
	static EconomySeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_ECONOMY_HPP */
