// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_VENDING_HPP
#define SEAM_VENDING_HPP

// [SATHENA-SEAM interface] VendingFeeSeam — onVendingTax fires inside vending_calc_tax with the
// vanilla-resolved rate in basis points (battle_config.vending_tax, or 0 when the vanilla guard
// fails) mutable by-ref; the caller then applies zeny -= zeny*(rate/10000). PLACEMENT: after the
// vanilla rate is resolved, before it is applied. Default no-op => vanilla rate.

class map_session_data;

struct VendingFeeSeam{
	virtual void onVendingTax( map_session_data* sd, double zeny, double& tax_rate ){}

	virtual ~VendingFeeSeam() = default;
};

inline VendingFeeSeam*& vending_seam(){
	static VendingFeeSeam s_default;
	static VendingFeeSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_VENDING_HPP */
