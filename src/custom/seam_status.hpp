// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_STATUS_HPP
#define SEAM_STATUS_HPP

// [SATHENA-SEAM interface] StatusSeam — the status-lifecycle hooks (widest status dependency).
// onStatusStart fires when an SC has been committed (end of status_change_start): ensemble
// mutex, on-inflict mirror-to-self, umbrella-immunity granting. onStatusEnd fires in
// status_change_end after the stale-timer guard, BEFORE removal — return false to VETO the
// removal (NoDispel / NoRemoveOnDead). `forced` distinguishes a forced end (dispel/death/
// manual: tid == INVALID_TIMER) from natural timer expiry, so a consumer scopes its veto to
// forced ends only and never freezes a timer. Default: no-op start / allow-all end => vanilla.

#include "../common/cbasetypes.hpp"   // int32 / int64

class block_list;

struct StatusSeam{
	virtual void onStatusStart( block_list* bl, int32 type, int32 val1, int64 duration ){}
	virtual bool onStatusEnd( block_list* bl, int32 type, bool forced ){ return true; }

	virtual ~StatusSeam() = default;
};

inline StatusSeam*& status_seam(){
	static StatusSeam s_default;
	static StatusSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_STATUS_HPP */
