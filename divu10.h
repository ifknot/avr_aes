/*
 * divu10.h
 *
 * Created: 4/1/2020 6:40:37 PM
 *  Author: ifkno
 */ 


#ifndef DIVU10_H_
#define DIVU10_H_

namespace math {

	/**
	 * divide by 10 using only bit shifts
	 * AVR does not have the ability to do hardware division i.e. no division instructions
	 * therefore subroutines must be written using other instructions to accomplish this task
	 */
	uint32_t divu10(uint32_t n);

}

#endif /* DIVU10_H_ */