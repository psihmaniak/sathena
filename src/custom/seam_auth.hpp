// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_AUTH_HPP
#define SEAM_AUTH_HPP

// [SATHENA-SEAM interface] AuthSeam — the packet-egress + login-session policy seam.
// onPacketSend observes every outgoing packet (WFIFOSET); onAccountAlreadyOnline fires at the
// login dup-login gate (return true to SPARE the existing session). Defaults are no-ops (pure
// vanilla). A consumer build replaces the instance via auth_seam() = &my_instance; from a
// static registrar or early init. The accessor uses function-local statics so registration
// order across translation units is safe.

#include <cstddef>
#include <cstdint>

#include "../common/cbasetypes.hpp"   // uint32 / int32

struct AuthSeam{
	// Fires once per packet enqueued for sending (WFIFOSET) — buf points at the packet
	// start (packet id = first 2 bytes, little-endian), len = the full packet length.
	// Observation only: the buffer is const; mutating egress is NOT this seam's job.
	virtual void onPacketSend( int32_t fd, const unsigned char* buf, size_t len ){}

	// Fires at the login dup-login gate; return true to SPARE the existing session (skip the
	// kick+reject) so a persistent background session survives a re-login. Default false =>
	// vanilla kick+reject.
	virtual bool onAccountAlreadyOnline( uint32 account_id, int32 char_server ){ return false; }

	virtual ~AuthSeam() = default;
};

inline AuthSeam*& auth_seam(){
	static AuthSeam s_default;
	static AuthSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_AUTH_HPP */
