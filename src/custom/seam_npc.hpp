// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_NPC_HPP
#define SEAM_NPC_HPP

// [SATHENA-SEAM interface] NpcSeam — the script-load extension point. onRegisterScripts()
// fires after the vanilla npc file list is built (scripts_main.conf chain) and BEFORE the
// files are parsed, so a consumer registers its own scripts via the existing extern
// npc_addsrcfile()/npc_delsrcfile() (add/override/disable) — paths relative to the run dir.
// Default no-op => pure vanilla script set.

struct NpcSeam{
	virtual void onRegisterScripts(){}

	virtual ~NpcSeam() = default;
};

inline NpcSeam*& npc_seam(){
	static NpcSeam s_default;
	static NpcSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_NPC_HPP */
