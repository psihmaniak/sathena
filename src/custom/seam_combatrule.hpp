// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_COMBATRULE_HPP
#define SEAM_COMBATRULE_HPP

// [SATHENA-SEAM interface] CombatRuleSeam — the "can src attack target" decision. onCheckTarget
// fires at the top of battle_check_target; return true to OVERRIDE the result (out = the
// battle_check_target return value: 1 allow / 0 or -1 deny), false to fall through to vanilla.
// Powers dynamic per-player killability (assassin contracts), custom PVP rules, area/faction
// gates — the rule state is DATA (char register / table) read in the consumer. Default = vanilla.

#include "../common/cbasetypes.hpp"   // int32

class block_list;

struct CombatRuleSeam{
	virtual bool onCheckTarget( const block_list* src, const block_list* target, int32 flag, int32& out ){ return false; }

	virtual ~CombatRuleSeam() = default;
};

inline CombatRuleSeam*& combat_rule_seam(){
	static CombatRuleSeam s_default;
	static CombatRuleSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_COMBATRULE_HPP */
