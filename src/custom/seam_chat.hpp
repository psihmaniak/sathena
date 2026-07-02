// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_CHAT_HPP
#define SEAM_CHAT_HPP

// [SATHENA-SEAM interface] ChatSeam — the chat pipeline. onChatSend fires after a player chat
// line is validated in clif_process_message (name/message are MUTABLE buffers → rewrite in
// place for swear/RMT/format; return false to BLOCK the line). onChatDeliver fires at the top
// of clif_GlobalMessage (area delivery) — the point a consumer can extend per-player ignore or
// run the i18n translation override. Default = pass-through / no-op => vanilla chat.

class map_session_data;
class block_list;

struct ChatSeam{
	virtual bool onChatSend( map_session_data* sd, char* name, char* message ){ return true; }
	virtual void onChatDeliver( const block_list& bl, const char* message ){}

	virtual ~ChatSeam() = default;
};

inline ChatSeam*& chat_seam(){
	static ChatSeam s_default;
	static ChatSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_CHAT_HPP */
