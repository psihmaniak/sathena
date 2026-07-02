// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_BOOT_HPP
#define SEAM_BOOT_HPP

// [SATHENA-SEAM interface] BootSeam — the shared boot-path + shared-DB-loader seam.
// Defaults are no-ops (pure vanilla). A consumer build replaces the instance via
//   boot_seam() = &my_instance;
// from a static registrar or early init. The accessor uses function-local statics so
// registration order across translation units is safe.

#include <string>

#include "../common/core.hpp"

class YamlDatabase;

struct BootSeam{
	// Fires ONCE per server process inside Core::start(), immediately after the server
	// subclass' initialize() succeeded (every DB/script/config is loaded) and BEFORE the
	// main runtime cycle serves anything. type = which server this process is.
	virtual void onServerBoot( rathena::server_core::e_core_type type ){}

	// Fires inside YamlDatabase::load() after the default location loaded, BEFORE
	// loadingFinished(). db_type = the database's type string ("ITEM_DB", "JOB_STATS", ...).
	// Consumers may call db.load( path ) here to merge additional files — loaded last, so
	// their entries override vanilla (the content-load mechanism).
	virtual void onDatabaseLoad( const std::string& db_type, YamlDatabase& db ){}

	virtual ~BootSeam() = default;
};

inline BootSeam*& boot_seam(){
	static BootSeam s_default;
	static BootSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_BOOT_HPP */
