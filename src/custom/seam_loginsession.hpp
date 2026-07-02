// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_LOGINSESSION_HPP
#define SEAM_LOGINSESSION_HPP

// [SATHENA-SEAM interface] LoginSessionSeam — the login-server half of the session-policy
// family (pairs with CharSessionSeam on the char server). onAccountAlreadyOnline fires at the
// login dup-login gate; return true to SPARE the existing session (skip the kick+reject) so a
// player can re-login while their autotrader/shared session persists. Default false => vanilla
// kick+reject. Autotrade-coexist needs BOTH this and CharSessionSeam to spare, plus the
// online-ledger extension (deferred content).

#include "../common/cbasetypes.hpp"   // uint32 / int32

struct LoginSessionSeam{
	virtual bool onAccountAlreadyOnline( uint32 account_id, int32 char_server ){ return false; }

	virtual ~LoginSessionSeam() = default;
};

inline LoginSessionSeam*& login_session_seam(){
	static LoginSessionSeam s_default;
	static LoginSessionSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_LOGINSESSION_HPP */
