// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_ECONOMY_HPP
#define SEAM_ECONOMY_HPP

// [SATHENA-SEAM interface] EconomySeam — zeny-flow observation/side-effects. onZenyChange fires
// after every pc_getzeny/pc_payzeny applies, with the SIGNED applied delta (+gained / -paid)
// and the log-type. In-engine consumer computes the guild/castle economy cut and credits a
// server-side fund store (no bus round-trip). Default no-op => vanilla.

#include "../common/cbasetypes.hpp"   // int32

class map_session_data;

struct EconomySeam{
	virtual void onZenyChange( map_session_data* sd, int32 delta, int32 log_type ){}

	virtual ~EconomySeam() = default;
};

inline EconomySeam*& economy_seam(){
	static EconomySeam s_default;
	static EconomySeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_ECONOMY_HPP */
