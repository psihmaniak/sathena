// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_CHARACTER_HPP
#define SEAM_CHARACTER_HPP

// [SATHENA-SEAM interface] CharacterSeam — the character stat/status/resource pipeline:
// custom-bonus dispatch (onBonus..onBonus5), status-lifecycle hooks (onStatusStart / onStatusEnd
// veto), the HP/SP resource pipeline (onItemHeal), the natural-regen tick (onRegenTick), and the
// derived-formula post-adjust (onStatusCalc). Defaults are no-ops (pure vanilla). A consumer
// build replaces the instance via character_seam() = &my_instance; from a static registrar or
// early init. The accessor uses function-local statics so registration order is safe.

#include "../common/cbasetypes.hpp"   // int32 / int64 / uint32

class map_session_data;
class block_list;
struct regen_data;

struct CharacterSeam{
	// custom-bonus dispatch — bonus ids unknown to vanilla route here from the pc_bonusN
	// default arms; return true = consumed (suppress the unknown-bonus warning).
	virtual bool onBonus ( map_session_data* sd, int32 type, int32 val ){ return false; }
	virtual bool onBonus2( map_session_data* sd, int32 type, int32 type2, int32 val ){ return false; }
	virtual bool onBonus3( map_session_data* sd, int32 type, int32 type2, int32 type3, int32 val ){ return false; }
	virtual bool onBonus4( map_session_data* sd, int32 type, int32 type2, int32 type3, int32 type4, int32 val ){ return false; }
	virtual bool onBonus5( map_session_data* sd, int32 type, int32 type2, int32 type3, int32 type4, int32 type5, int32 val ){ return false; }

	// an SC has been committed (end of status_change_start).
	virtual void onStatusStart( block_list* bl, int32 type, int32 val1, int64 duration ){}
	// status_change_end after the stale-timer guard, BEFORE removal — return false to VETO.
	virtual bool onStatusEnd( block_list* bl, int32 type, bool forced ){ return true; }

	// final HP/SP heal adjustment at the end of pc_itemheal; itemid is t_itemid (== uint32).
	virtual void onItemHeal( map_session_data* sd, uint32 itemid, int32& hp, int32& sp ){}

	// natural HP/SP regen tick (status_natural_heal), BEFORE regen flag/rates are read.
	virtual void onRegenTick( block_list* bl, regen_data* regen ){}

	// tail of status_calc_bl_main, after every stat field has been recomputed (bl by reference).
	virtual void onStatusCalc( block_list& bl ){}

	virtual ~CharacterSeam() = default;
};

inline CharacterSeam*& character_seam(){
	static CharacterSeam s_default;
	static CharacterSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_CHARACTER_HPP */
