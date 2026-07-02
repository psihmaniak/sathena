// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_RESOURCE_HPP
#define SEAM_RESOURCE_HPP

// [SATHENA-SEAM interface] ResourceSeam — the HP/SP resource pipeline. onItemHeal fires at the
// end of pc_itemheal, after all vanilla heal-rate bonuses, with the FINAL hp/sp mutable by
// reference. (Regen interval/amount + skill HP/SP cost are sibling hooks in status_natural_heal
// / skill_consume.) Default no-op => vanilla.

#include "../common/cbasetypes.hpp"   // int32 / uint32

class map_session_data;

struct ResourceSeam{
	// itemid is t_itemid (== uint32); use the concrete type so the header needs no mmo.hpp.
	virtual void onItemHeal( map_session_data* sd, uint32 itemid, int32& hp, int32& sp ){}

	virtual ~ResourceSeam() = default;
};

inline ResourceSeam*& resource_seam(){
	static ResourceSeam s_default;
	static ResourceSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_RESOURCE_HPP */
