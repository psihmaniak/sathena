// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_FOLLOWER_HPP
#define SEAM_FOLLOWER_HPP

// [SATHENA-SEAM interface] FollowerSeam — the pet/homunculus follower family. onLogFeeding taps
// every feeding event (type = e_log_feeding_type, nameid = food item). Default no-op => the
// vanilla sink is unchanged. A consumer build replaces the instance via follower_seam() =
// &my_instance; the accessor uses function-local statics so registration order is safe.

#include "../common/cbasetypes.hpp"   // int32 / uint32

class map_session_data;

struct FollowerSeam{
	// pet/homunculus feeding. type = e_log_feeding_type, nameid = food item.
	virtual void onLogFeeding( const map_session_data& sd, int32 type, uint32 nameid ){}

	virtual ~FollowerSeam() = default;
};

inline FollowerSeam*& follower_seam(){
	static FollowerSeam s_default;
	static FollowerSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_FOLLOWER_HPP */
