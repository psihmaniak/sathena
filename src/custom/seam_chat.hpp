// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_CHAT_HPP
#define SEAM_CHAT_HPP

// [SATHENA-SEAM interface] ChatSeam — the chat pipeline. onChatSend fires after a player chat
// line is validated in clif_process_message (name/message are MUTABLE buffers → rewrite in
// place for swear/RMT/format; return false to BLOCK the line). onChatDeliver fires at the top
// of clif_GlobalMessage (area delivery — per-player ignore / i18n override). onLogChat taps
// every chat line at the log_chat sink. Defaults = pass-through / no-op => vanilla chat. A
// consumer build replaces the instance via chat_seam() = &my_instance; the accessor uses
// function-local statics so registration order is safe.

#include "../common/cbasetypes.hpp"   // int32

class map_session_data;
class block_list;

struct ChatSeam{
	virtual bool onChatSend( map_session_data* sd, char* name, char* message ){ return true; }
	virtual void onChatDeliver( const block_list& bl, const char* message ){}

	// chat line at the log sink. type = e_log_chat_type, type_id = channel/target id, src_* =
	// speaker, mapname/x/y = origin, dst_charname = whisper target (may be null pre-gate).
	virtual void onLogChat( int32 type, int32 type_id, int32 src_charid, int32 src_accid, const char* mapname, int32 x, int32 y, const char* dst_charname, const char* message ){}

	virtual ~ChatSeam() = default;
};

inline ChatSeam*& chat_seam(){
	static ChatSeam s_default;
	static ChatSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_CHAT_HPP */
