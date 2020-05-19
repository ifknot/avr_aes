/*
 * block_cipher_constants.h
 *
 * Created: 5/8/2020 12:03:41 PM
 *  Author: ifkno
 */ 


#ifndef BLOCK_CIPHER_CONSTANTS_H_
#define BLOCK_CIPHER_CONSTANTS_H_

namespace avr_aes {

	/**
     * 128-bit block size.
     * Until the announcement of NIST's AES contest, the majority of block ciphers followed the example of the DES in using a block size of 64 bits (8 bytes).
     * However, the birthday paradox tells us that after accumulating a number of blocks equal to the square root of the total number possible,
     * there will be an approximately 50% chance of two or more being the same, which would start to leak information about the message contents.
     * Consequently, AES candidates were required to support a block length of 128 bits (16 bytes).
     * This should be acceptable for up to 2^64 × 16 B = 256 exabytes of data - which should suffice for quite a few years to come.
     * The winner of the AES contest, Rijndael, supports block and key sizes of 128, 192, and 256 bits, but in AES the block size is always 128 bits.
     */
    constexpr static size_t BLOCK_SIZE = 16;
	
	/**
	 * NIST block length of 128 bits (16 bytes)
	 */
	typedef uint8_t nist_block_t[BLOCK_SIZE];

    constexpr static size_t BYTES_PER_BLOCK = BLOCK_SIZE * 8;

    constexpr static size_t WORD_SIZE = 4; //bytes

    /**
     * Nonce size (bytes)
     * @warning An 8 byte nonce is not secure as a general recommendation.
     * @note CCM (which uses CTR), RFC5084/CMS recommends 12 bytes for the nonce.
     */
    constexpr static size_t NONCE_SIZE = 12;
	
}

#endif /* BLOCK_CIPHER_CONSTANTS_H_ */