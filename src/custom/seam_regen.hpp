// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_REGEN_HPP
#define SEAM_REGEN_HPP

// [SATHENA-SEAM interface] RegenSeam — the natural HP/SP regen tick (status_natural_heal).
// onRegenTick fires each tick with the mutable regen_data, BEFORE its flag/rates are read, so a
// consumer reworks amounts/rates (x4 amount, Hide x3, homun /3) and toggles regen types by
// editing regen->flag / regen->rate. (Tick INTERVALS are battle_config.natural_heal_*_interval
// = pure conf, no seam.) Default no-op => vanilla regen. regen_data is forward-declared; the
// consumer includes status.hpp to touch its fields.

class block_list;
struct regen_data;

struct RegenSeam{
	virtual void onRegenTick( block_list* bl, regen_data* regen ){}

	virtual ~RegenSeam() = default;
};

inline RegenSeam*& regen_seam(){
	static RegenSeam s_default;
	static RegenSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_REGEN_HPP */
