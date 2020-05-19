/*
 * require.cpp
 *
 * Created: 4/14/2020 8:03:05 PM
 *  Author: ifkno
 */ 

#include "tiny_catch.h"

namespace err {
	
	uint32_t last_error = 0;
	uint16_t error_count = 0;
	uint16_t test_count = 0;
	
	void abort() {
		#ifdef LOG_ERRORS
		// TODO: use eeprom device to log abort code
		#endif
		while(true); // safe predictable loud error
	}

	void require_action(uint8_t version, uint8_t id, uint16_t line) {
		check_action(version, id, line);
		abort();
	}
	
	void check_action(uint8_t version, uint8_t id, uint16_t line) {
		++error_count;
		last_error = version;
		last_error <<= 8;
		last_error |= id;
		last_error <<= 16;
		last_error |= line;
		#ifdef LOG_ERRORS
		// TODO: use eeprom device to log error code
		#endif
	}

}
