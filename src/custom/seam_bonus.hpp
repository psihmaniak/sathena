// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_BONUS_HPP
#define SEAM_BONUS_HPP

// [SATHENA-SEAM interface] BonusSeam — the custom-bonus dispatch. Each method fires from
// the default: arm of the matching pc_bonusN when the bonus id is unknown to vanilla.
// Return true = consumed (suppresses the unknown-bonus warning); false = vanilla warning.
// Consumers own the id space above vanilla's SP_ enums and their own storage (side
// tables — never grow map_session_data).

#include "../common/cbasetypes.hpp"

class map_session_data;

struct BonusSeam{
	virtual bool onBonus ( map_session_data* sd, int32 type, int32 val ){ return false; }
	virtual bool onBonus2( map_session_data* sd, int32 type, int32 type2, int32 val ){ return false; }
	virtual bool onBonus3( map_session_data* sd, int32 type, int32 type2, int32 type3, int32 val ){ return false; }
	virtual bool onBonus4( map_session_data* sd, int32 type, int32 type2, int32 type3, int32 type4, int32 val ){ return false; }
	virtual bool onBonus5( map_session_data* sd, int32 type, int32 type2, int32 type3, int32 type4, int32 type5, int32 val ){ return false; }

	virtual ~BonusSeam() = default;
};

inline BonusSeam*& bonus_seam(){
	static BonusSeam s_default;
	static BonusSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_BONUS_HPP */
