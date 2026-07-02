// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_SKILL_HPP
#define SEAM_SKILL_HPP

// [SATHENA-SEAM interface] SkillSeam — the custom SkillImpl provider. SkillFactoryCustom
// (first in the factory list, "always first to allow overwriting skills") delegates here,
// so a consumer supplies/overrides skill behavior by returning a SkillImpl for a skill_id
// — no edit to the factory list, no touch to any vanilla skill_factory_*.cpp.
// Default returns nullptr => vanilla behavior (the factory falls through to the built-in
// per-class factories). Consumer impls subclass SkillImpl (or a specific SkillXxx base) in
// thenro/src and are compiled in via the build seam.

#include <memory>

#include "../map/skills/skill_factory.hpp"   // SkillImpl, e_skill

struct SkillSeam{
	virtual std::unique_ptr<const SkillImpl> create( e_skill skill_id ) const{ return nullptr; }

	virtual ~SkillSeam() = default;
};

inline SkillSeam*& skill_seam(){
	static SkillSeam s_default;
	static SkillSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_SKILL_HPP */
