// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_MOB_HPP
#define SEAM_MOB_HPP

// [SATHENA-SEAM interface] MobSeam — a per-mob AI-tick override. onAiTick fires each 100ms think
// cycle after the vanilla early-outs (mob alive, thinktime elapsed, not stunned) and BEFORE the
// vanilla skill+target cascade; return true to REPLACE this tick's AI, false to run vanilla.
// Default no-op (returns false) => pure vanilla AI. A consumer build replaces the instance via
// mob_seam() = &my_instance; the accessor uses function-local statics so registration order is safe.

#include "../common/timer.hpp"   // t_tick

class mob_data;

struct MobSeam{
	virtual bool onAiTick( mob_data* md, t_tick tick ){ return false; }

	virtual ~MobSeam() = default;
};

inline MobSeam*& mob_seam(){
	static MobSeam s_default;
	static MobSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_MOB_HPP */
