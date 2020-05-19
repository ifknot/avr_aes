/*
 * aes128_types.h
 *
 * Created: 5/8/2020 10:26:59 PM
 *  Author: ifkno
 */ 

#ifndef AES_TYPES_H_
#define AES_TYPES_H_

#include "aes_constants.h"
#include "nist_block_cipher_constants.h"

namespace avr_aes {
	
	/**
     * R as the round number - round keys needed:
     * + 11 round keys for AES-128
     * + 13 keys for AES-192
     * + 15 keys for AES-256
     */
	constexpr uint8_t R = R128;
	
	/**
     * N as the length of the key in 32-bit words:
     * + 4 words AES-128
     * + 6 words AES-192
     * + 8 words AES-256
     */
	constexpr uint8_t N = N128;
	
	/**
     * XK as the length of the expanded key in 8-bit bytes.
     */
    constexpr static size_t XK = R * N * WORD_SIZE;
	
	/**
	 * A 128-bit key requires an array of 16 bytes
	 */
	typedef uint8_t key_t[N * WORD_SIZE];
	
	/**
	 * Expanding a 128-bit key requires an array of 176 bytes
	 * i.e. 16 bytes for each of the 11 rounds
	 */
	typedef uint8_t expanded_key_t[XK];
	
	
		
}

#endif /* AES_TYPES_H_ */