// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_BATTLE_HPP
#define SEAM_BATTLE_HPP

// [SATHENA-SEAM interface] BattleSeam — the combat pipeline: the staged damage-calc framework
// (onDamageStage, tagged by e_damage_stage), the applied-damage choke (onDamageTaken, last word
// before HP is deducted), the can-attack decision (onCheckTarget), and the kill / skill-use
// realtime chokes (onMobDead / onSkillUse). Defaults are no-ops (pure vanilla). A consumer build
// replaces the instance via battle_seam() = &my_instance; the accessor uses function-local
// statics so registration order is safe.

#include "../common/cbasetypes.hpp"   // int32 / int64 / uint16
#include "../map/battle.hpp"          // struct Damage, block_list

class mob_data;

enum e_damage_stage{
	DMG_STAGE_SIZE,
	DMG_STAGE_MASTERY,
	DMG_STAGE_SKILLRATIO,
	DMG_STAGE_DEF,
	DMG_STAGE_ELEMENT,
	DMG_STAGE_CARDS,
	DMG_STAGE_FINAL,       // battle_calc_damage — merged number, both SCs live
};

struct BattleSeam{
	// staged weapon-damage calc — one virtual per stage boundary; `damage` is mutable by ref.
	virtual void onDamageStage( e_damage_stage stage, block_list* src, block_list* target,
			const Damage* d, int64& damage, uint16 skill_id, uint16 skill_lv ){}

	// applied-damage choke (status_damage): hp about to be deducted (capped to cur_hp), mutable.
	virtual void onDamageTaken( block_list* src, block_list* target, int32& hp, int32 cur_hp,
			int32 flag, uint16 skill_id ){}

	// can-attack decision (top of battle_check_target); return true to OVERRIDE (out = verdict).
	virtual bool onCheckTarget( const block_list* src, const block_list* target, int32 flag, int32& out ){ return false; }

	// a mob just died — src = killer bl (nullptr if script/command), type = death-flag bitfield.
	virtual void onMobDead( mob_data* md, block_list* src, int32 type ){}

	// a skill landed on a target (from the castend damage + no-damage dispatch).
	virtual void onSkillUse( block_list* src, block_list* target, uint16 skill_id, uint16 skill_lv ){}

	virtual ~BattleSeam() = default;
};

inline BattleSeam*& battle_seam(){
	static BattleSeam s_default;
	static BattleSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_BATTLE_HPP */
