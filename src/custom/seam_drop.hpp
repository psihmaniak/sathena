// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_DROP_HPP
#define SEAM_DROP_HPP

// [SATHENA-SEAM interface] DropSeam — per-context drop-rate decision. onDropRate fires inside
// mob_getdroprate (every drop's rate flows through it) with the killer + dying mob; the
// consumer mutates the final rate (0..10000 basis points) in place — drop-by-killer/context,
// event multipliers, and punishment (0 drops for a flagged player). Default no-op => vanilla.
// Item-specific decisions (conditional drops, champion-max-options) attach later at the
// per-item drop loop; this covers the rate for every drop.

#include "../common/cbasetypes.hpp"   // int32

class block_list;
class mob_data;

struct DropSeam{
	virtual void onDropRate( block_list* killer, mob_data* md, int32& drop_rate ){}

	virtual ~DropSeam() = default;
};

inline DropSeam*& drop_seam(){
	static DropSeam s_default;
	static DropSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_DROP_HPP */
