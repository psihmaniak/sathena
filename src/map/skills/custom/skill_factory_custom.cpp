// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "skill_factory_custom.hpp"

#include "../swordman/bash.hpp"

#ifdef SATHENA
// [SATHENA-SEAM] SkillSeam — delegate the custom-skill slot to the consumer. PLACEMENT:
// SkillFactoryCustom is already the first factory tried ("always first to allow
// overwriting skills"), so consulting the seam here overrides/adds any skill without
// editing the factory list. A non-null result wins; nullptr falls through to vanilla.
#include <custom/seam_skill.hpp>
#endif

class SkillCustomBash;

std::unique_ptr<const SkillImpl> SkillFactoryCustom::create(const e_skill skill_id) const {
#ifdef SATHENA
	if( std::unique_ptr<const SkillImpl> impl = skill_seam()->create( skill_id ); impl != nullptr ){
		return impl;
	}
#endif
#if 0
	switch( skill_id ){
		case SM_BASH:
			return std::make_unique<SkillCustomBash>();

		default:
			return nullptr;
	}
#endif

	return nullptr;
}

class SkillCustomBash : public SkillBash{
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override{
		// Normal Bash:
		// Base 100% + 30% per level
		// base_skillratio += 30 * skill_lv;

		// But my custom Bash Skill is stronger:
		// Base 100% + 300% per level
		base_skillratio += 300 * skill_lv;
	}
};
