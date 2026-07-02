// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_FORMULA_HPP
#define SEAM_FORMULA_HPP

// [SATHENA-SEAM interface] FormulaSeam — post-adjust the derived combat formulas. onStatusCalc
// fires at the tail of status_calc_bl_main, after every stat field has been recomputed, with bl
// by reference (a consumer edits bl's computed battle_status in place). onCastTime fires on the
// return of skill_vfcastfix and onSkillDelay on the return of skill_delayfix; each passes the
// computed int by reference to be mutated before it is returned. Default no-op => vanilla.

#include "../common/cbasetypes.hpp"   // int32 / uint16

class block_list;

struct FormulaSeam{
	virtual void onStatusCalc( block_list& bl ){}
	virtual void onCastTime( block_list* bl, int32& time, uint16 skill_id, uint16 skill_lv ){}
	virtual void onSkillDelay( block_list* bl, int32& delay, uint16 skill_id, uint16 skill_lv ){}

	virtual ~FormulaSeam() = default;
};

inline FormulaSeam*& formula_seam(){
	static FormulaSeam s_default;
	static FormulaSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_FORMULA_HPP */
