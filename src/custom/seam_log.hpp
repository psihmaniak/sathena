// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_LOG_HPP
#define SEAM_LOG_HPP

// [SATHENA-SEAM interface] LogSeam — the audit-stream tap. Each hook fires at the TOP of the
// matching log_* function, BEFORE the enable_logs / sql_logs gate, so a consumer receives
// every event INDEPENDENTLY of whether the vanilla SQL/file sink is on. Default no-op => the
// vanilla sink runs as configured, unchanged. Hooks are added per log_* as needed.

#include "../common/cbasetypes.hpp"   // int32 / int16 / uint32

struct item;
class map_session_data;

struct LogSeam{
	// item flow (pickup/drop/use/trade/storage/...). id = char/account id, m = mapid,
	// type = e_log_pick_type, itm carries nameid + unique_id (the correlation key).
	virtual void onLogPick( int32 id, int16 m, int32 type, int32 amount, const item* itm ){}

	// zeny flow. type = e_log_pick_type, src_id = counterparty, amount signed.
	virtual void onLogZeny( const map_session_data& sd, int32 type, uint32 src_id, int32 amount ){}

	virtual ~LogSeam() = default;
};

inline LogSeam*& log_seam(){
	static LogSeam s_default;
	static LogSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_LOG_HPP */
