/*
 * tiny_catch.h compact Behaviour Driven Development (BDD) 
 *
 * Created: 4/14/2020 7:17:15 PM
 *  Author: ifkno
 */ 

#include <stdint.h>

#ifndef REQUIRE_H_
#define REQUIRE_H_

/**
 * macro to set the software version
 * used to construct the error code
 * compile error unless set
 */
#define SET_VERSION(v) enum { _VERSION_ = v };

/**
 * macro to set the id to replace the _FILE_ string constant
 * placed at the top of every source file
 * the dummy __unique function concatenated with ## causes the linker to throw an error if the same file id is used more than once
 * compile error unless set and unique
 */
#define SET_FILE_ID(id) \
	enum { _FILE_ID_ = id }; \
	void __unique##id(void) {}		

/**
 * Aborting test assertion require macro
 * calls require_action with args to construct a localized error code
 * require_action should call abort();
 */
#define REQUIRE(expr) \
	++err::test_count; \
	if ((expr)) {} \
	else { \
		err::require_action(_VERSION_, _FILE_ID_, __LINE__); \
		uart.write('@'); \
		uart.write_dec(err::last_error & 0xFFFF); \
		uart.write_endl(); \
	}

/**
 * Continuation test assertion check macro
 * calls check_action with args to construct a localized error code
 */	
#define CHECK(expr) \
	++err::test_count; \
	if ((expr)) {} \
	else { \
		err::check_action(_VERSION_, _FILE_ID_, __LINE__); \
		uart.write('@'); \
		uart.write_dec(err::last_error & 0xFFFF); \
		uart.write_endl(); \
	}
	
#define REPORT(msg) \
	uart.write_endl(); \
	uart.write_dec(err::test_count - err::error_count); \
	uart.write(" test(s) passed "); \
	if(!err::error_count) { \
		uart.write(" all tests passed :) for "); \
		uart.write(msg); \
	} \
	else { \
		uart.write_dec(err::error_count); \
		uart.write(" failed test(s) :( for "); \
		uart.write(msg); \
	}
	
	
namespace err {
	
	extern uint32_t last_error;
	extern uint16_t error_count;
	extern uint16_t test_count;
	
	/**
	 * 
	 */
	void abort();
	
	/**
	 * Rather than filling up the program memory with constant strings from _FILE_ and failed expression info
	 * construct a 32 bit error code from:
	 * 1. software version number	8 bits	(otherwise the line number will be meaningless) 
	 * 2. the file id number		8 bits	(to avoid filename string constants)
	 * 3. the line number			16 bits
	 *
	 * There are then a number of fail action possibilities e.g.
	 * + saving the error code in eeprom 
	 * + abort with error to serial
	 * + enable login via a debug task for system interrogation
	 */
	void require_action(uint8_t version, uint8_t id, uint16_t line);
	
	void check_action(uint8_t version, uint8_t id, uint16_t line);
}


#endif /* REQUIRE_H_ */

/*
#include "tiny_catch.h"

SET_VERSION(0x11);  // version 1.1
SET_FILE_ID(0x01);  // unique file id 1

#include "uart_device.h"

int main() {
	
	auto& uart = singleton::uart_device::instance(); // NB tiny_catch expects uart to be defined
	
	//SCENARIO some things to test
	
	//GIVEN those things
	
	CHECK( 1 == 1 );
	CHECK( false );
	
	//WHEN something else happens
	
	//THEN there are expected consequences
	CHECK( true);
	//REQUIRE( 2 < 0 ); //calls abort
	
	REPORT("jst");
	
	
}

*/