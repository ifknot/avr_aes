/*
 * divu10.cpp
 *
 * Created: 4/1/2020 6:41:59 PM
 *  Author: ifkno
 */ 

#include <avr/io.h>

namespace math {
	
	//solution heavily inspired by Hacker's Delight that really uses only bit shifts
	uint32_t divu10(uint32_t n) {
		uint32_t q, r;
		q = (n >> 1) + (n >> 2);
		q = q + (q >> 4);
		q = q + (q >> 8);
		q = q + (q >> 16);
		q = q >> 3;
		r = n - (((q << 2) + q) << 1);
		return q + (r > 9);
	}
	
}