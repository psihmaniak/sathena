// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_MOBAI_HPP
#define SEAM_MOBAI_HPP

// [SATHENA-SEAM interface] MobAISeam — a per-mob AI-tick override. onAiTick fires each 100ms
// think cycle after the vanilla early-outs (mob alive, thinktime elapsed, not stunned) and
// BEFORE the vanilla skill+target cascade; return true to REPLACE this tick's vanilla AI,
// false to run vanilla. Opt-in is the consumer's job — it keys on mob_id / a side-table (NOT
// a mob_data field → no struct drift), so 99% of mobs pay one virtual call and fall straight
// through. IN-ENGINE only (per-mob/100ms budget). Default no-op (returns false) => pure vanilla AI.

#include "../common/timer.hpp"   // t_tick

class mob_data;

struct MobAISeam{
	virtual bool onAiTick( mob_data* md, t_tick tick ){ return false; }

	virtual ~MobAISeam() = default;
};

inline MobAISeam*& mob_ai_seam(){
	static MobAISeam s_default;
	static MobAISeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_MOBAI_HPP */
