// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_CONTENT_HPP
#define SEAM_CONTENT_HPP

// [SATHENA-SEAM interface] ContentSeam — the content-foundation seam: the shared boot-path +
// shared-DB-loader hooks (onServerBoot / onDatabaseLoad), the script-load extension point
// (onRegisterScripts), and the two npc log taps (onLogNpc / onLogNpcData). Defaults are no-ops
// (pure vanilla). A consumer build replaces the instance via content_seam() = &my_instance;
// from a static registrar or early init. The accessor uses function-local statics so
// registration order across translation units is safe.

#include <string>

#include "../common/core.hpp"

class YamlDatabase;
class map_session_data;
struct npc_data;

struct ContentSeam{
	// Fires ONCE per server process inside Core::start(), immediately after the server
	// subclass' initialize() succeeded (every DB/script/config is loaded) and BEFORE the
	// main runtime cycle serves anything. type = which server this process is.
	virtual void onServerBoot( rathena::server_core::e_core_type type ){}

	// Fires inside YamlDatabase::load() after the default location loaded, BEFORE
	// loadingFinished(). db_type = the database's type string ("ITEM_DB", "JOB_STATS", ...).
	// Consumers may call db.loadExtra( path ) here to merge additional files — loaded last, so
	// their entries override vanilla (the content-load mechanism).
	virtual void onDatabaseLoad( const std::string& db_type, YamlDatabase& db ){}

	// Fires after the vanilla npc file list is built (scripts_main.conf chain) and BEFORE the
	// files are parsed, so a consumer registers its own scripts via the existing extern
	// npc_addsrcfile()/npc_delsrcfile() (add/override/disable) — paths relative to the run dir.
	virtual void onRegisterScripts(){}

	// script logmes with an attached player.
	virtual void onLogNpc( const map_session_data& sd, const char* message ){}

	// script logmes from an npc with no attached player.
	virtual void onLogNpcData( const npc_data& nd, const char* message ){}

	virtual ~ContentSeam() = default;
};

inline ContentSeam*& content_seam(){
	static ContentSeam s_default;
	static ContentSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_CONTENT_HPP */
