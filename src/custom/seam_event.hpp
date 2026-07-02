// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_EVENT_HPP
#define SEAM_EVENT_HPP

// [SATHENA-SEAM interface] EventSeam — realtime gameplay CHOKE-POINTS (kill/death/skill-use/
// level/login...). The consumer emits to the bus AND/OR runs in-engine logic (on-kill grants,
// champion counters, drop-by-context state). Distinct from LogSeam (the comprehensive audit
// stream) — EventSeam is curated realtime. Hooks are wired incrementally; each is one
// call-site. Default no-op => pure vanilla.

#include "../common/cbasetypes.hpp"   // int32

class mob_data;
class block_list;

struct EventSeam{
	// A mob just died. src = the killer bl (nullptr if script/command killed), type = the
	// death-flag bitfield. Fires once per death, before drops/exp are distributed, so a
	// consumer can grant/emit with the killer identified.
	virtual void onMobDead( mob_data* md, block_list* src, int32 type ){}

	virtual ~EventSeam() = default;
};

inline EventSeam*& event_seam(){
	static EventSeam s_default;
	static EventSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_EVENT_HPP */
