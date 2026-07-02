// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_NET_HPP
#define SEAM_NET_HPP

// [SATHENA-SEAM interface] NetSeam — packet-egress observation. Default = no-op.
// Consumers (e.g. a test build's packet tap) replace the instance via net_seam() = &impl;
// Hot path: one pointer deref + virtual call per outgoing packet — negligible, and the
// default body is empty.

#include <cstddef>
#include <cstdint>

struct NetSeam{
	// Fires once per packet enqueued for sending (WFIFOSET) — buf points at the packet
	// start (packet id = first 2 bytes, little-endian), len = the full packet length.
	// Observation only: the buffer is const; mutating egress is NOT this seam's job.
	virtual void onPacketSend( int32_t fd, const unsigned char* buf, size_t len ){}

	virtual ~NetSeam() = default;
};

inline NetSeam*& net_seam(){
	static NetSeam s_default;
	static NetSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_NET_HPP */
