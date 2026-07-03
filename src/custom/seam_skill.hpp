// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_SKILL_HPP
#define SEAM_SKILL_HPP

// [SATHENA-SEAM interface] SkillSeam — the custom SkillImpl provider plus the cast/after-cast
// formula post-adjust. create() delegates the custom-skill slot (SkillFactoryCustom is first in
// the factory list, so a non-null result overrides/adds any skill); onCastTime fires on the
// return of skill_vfcastfix and onSkillDelay on the return of skill_delayfix, each mutating the
// computed value by reference. Defaults are no-ops (create => nullptr, mutators leave the value).
// A consumer build replaces the instance via skill_seam() = &my_instance; the accessor uses
// function-local statics so registration order is safe.

#include <memory>

#include "../common/cbasetypes.hpp"          // int32 / uint16
#include "../map/skills/skill_factory.hpp"   // SkillImpl, e_skill

class block_list;

struct SkillSeam{
	// custom SkillImpl provider — nullptr falls through to the vanilla per-class factories.
	virtual std::unique_ptr<const SkillImpl> create( e_skill skill_id ) const{ return nullptr; }

	// computed cast time (return of skill_vfcastfix), mutable by reference.
	virtual void onCastTime( block_list* bl, int32& time, uint16 skill_id, uint16 skill_lv ){}
	// computed after-cast delay (return of skill_delayfix), mutable by reference.
	virtual void onSkillDelay( block_list* bl, int32& delay, uint16 skill_id, uint16 skill_lv ){}

	virtual ~SkillSeam() = default;
};

inline SkillSeam*& skill_seam(){
	static SkillSeam s_default;
	static SkillSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_SKILL_HPP */
