// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_COMBAT_OUTCOME_HPP
#define SEAM_COMBAT_OUTCOME_HPP

// [SATHENA-SEAM interface] CombatOutcomeSeam — the applied-damage choke (status_damage), the
// last word before HP is deducted. onDamageTaken sees the FINAL hp about to be removed (already
// capped to cur_hp, so hp >= cur_hp means this blow is lethal) and can mutate it in place.
// Default no-op => vanilla. (This is the applier; the battle_calc pipeline is DamageSeam.)

#include "../common/cbasetypes.hpp"   // int32 / uint16

class block_list;

struct CombatOutcomeSeam{
	virtual void onDamageTaken( block_list* src, block_list* target, int32& hp, int32 cur_hp,
			int32 flag, uint16 skill_id ){}

	virtual ~CombatOutcomeSeam() = default;
};

inline CombatOutcomeSeam*& combat_outcome_seam(){
	static CombatOutcomeSeam s_default;
	static CombatOutcomeSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_COMBAT_OUTCOME_HPP */
