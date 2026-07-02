// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_DAMAGE_HPP
#define SEAM_DAMAGE_HPP

// [SATHENA-SEAM interface] DamageSeam — the staged damage-pipeline framework. ONE virtual
// fires at each stage boundary of the weapon-damage calc, tagged by e_damage_stage; the
// consumer switches on the stage it wants (SEAM DESIGN PRINCIPLE: the modifier declares its
// stage). `damage` is mutable-by-reference — mutate in place, the pipeline continues with
// the new value. Default no-op => pure vanilla.
//
// Stages are wired incrementally (each is one call-site of the same virtual). FINAL is the
// context-richest choke — battle_calc_damage, sees BOTH attacker+target SC + flag + the
// merged damage number, and is also shared by magic/misc. Earlier per-bucket stages
// (size/mastery/ratio/def/element/cards) attach in battle_calc_weapon_attack as needed.

#include "../map/battle.hpp"   // struct Damage, block_list

enum e_damage_stage{
	DMG_STAGE_SIZE,
	DMG_STAGE_MASTERY,
	DMG_STAGE_SKILLRATIO,
	DMG_STAGE_DEF,
	DMG_STAGE_ELEMENT,
	DMG_STAGE_CARDS,
	DMG_STAGE_FINAL,       // battle_calc_damage — merged number, both SCs live
};

struct DamageSeam{
	virtual void onDamageStage( e_damage_stage stage, block_list* src, block_list* target,
			const Damage* d, int64& damage, uint16 skill_id, uint16 skill_lv ){}

	virtual ~DamageSeam() = default;
};

inline DamageSeam*& damage_seam(){
	static DamageSeam s_default;
	static DamageSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_DAMAGE_HPP */
