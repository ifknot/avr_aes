/*
 * aes_functions.h
 *
 * Created: 5/10/2020 5:27:11 PM
 *  Author: ifkno
 */ 


#ifndef AES_FUNCTIONS_H_
#define AES_FUNCTIONS_H_

#include "aes_types.h"

namespace avr_aes {
	
	void make_expanded_key(const key_t key, expanded_key_t xkey);
	
	/**
     * multiply by 2 in the Rijndael algo's Galois' Field (GF)
     * @param x
     * @return
     */
    static inline uint_fast32_t GF2(uint_fast32_t x) {
        return (x << 1u)			// implicitly removes high bit because 8-bit, (so * 0x1b and not 0x11b)
               ^					// XOR
               (((x >> 7u) & 1u)	// arithmetic right shift, thus shifting in either zeros or ones
                * 0x1bu);			// Rijndael's Galois field
    }
	
	/**
	 * @brief GF add (XOR) the round key to the block _increasing_ the round + 16
     * @note *has side effects* (advances to next round key) but saves time consuming multiplications
     * @param rkey const byte* to the round key offset
     * @param i iterator byte* to the block
     */
	void add_round_key(uint8_t &rkey, const expanded_key_t xkey, uint8_t* i);
	
	/**
     * @brief S-box substitution
     * @param i iterator byte* to the block
     */
    void sub_bytes(uint8_t* i);

    /**
     * @brief shifts the rows in the block to the left, each by a different offset.
     * @param i iterator byte* to the block
     */
   void shift_rows(uint8_t* i);

    /**
     * @brief consider 16 byte block as 4x4 matrix and mix columns as per Rijndael algorithm.
     * The operation consists in the modular multiplication of two four-term polynomials, whose coefficients are
     * elements of _GF(2^8)_. The modulo used for this operation is _x^4+1_.
     * During encryption the Mix Column the multiplication matrix is
     * 02 03 01 01
     * 01 02 03 01
     * 01 01 02 03
     * 03 01 01 02
     * @param i iterator byte* to the block
     */
    void mix_columns(uint8_t* i);
	
	/**
	 * take a pointer to an expanded key and an iterator to a 16 byte plain-text block
	 * encrypt in place the 16 byte plain-text block
	 */
	void encrypt(const expanded_key_t xkey, uint8_t* i);
	
}


#endif /* AES_FUNCTIONS_H_ */

/*
expanded key test vectors:
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00

00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
62 63 63 63 62 63 63 63 62 63 63 63 62 63 63 63
9b 98 98 c9 f9 fb fb aa 9b 98 98 c9 f9 fb fb aa
90 97 34 50 69 6c cf fa f2 f4 57 33 0b 0f ac 99
ee 06 da 7b 87 6a 15 81 75 9e 42 b2 7e 91 ee 2b
7f 2e 2b 88 f8 44 3e 09 8d da 7c bb f3 4b 92 90
ec 61 4b 85 14 25 75 8c 99 ff 09 37 6a b4 9b a7
21 75 17 87 35 50 62 0b ac af 6b 3c c6 1b f0 9b
0e f9 03 33 3b a9 61 38 97 06 0a 04 51 1d fa 9f
b1 d4 d8 e2 8a 7d b9 da 1d 7b b3 de 4c 66 49 41
b4 ef 5b cb 3e 92 e2 11 23 e9 51 cf 6f 8f 18 8e

ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff

ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff
e8 e9 e9 e9 17 16 16 16 e8 e9 e9 e9 17 16 16 16
ad ae ae 19 ba b8 b8 0f 52 51 51 e6 45 47 47 f0
09 0e 22 77 b3 b6 9a 78 e1 e7 cb 9e a4 a0 8c 6e
e1 6a bd 3e 52 dc 27 46 b3 3b ec d8 17 9b 60 b6
e5 ba f3 ce b7 66 d4 88 04 5d 38 50 13 c6 58 e6
71 d0 7d b3 c6 b6 a9 3b c2 eb 91 6b d1 2d c9 8d
e9 0d 20 8d 2f bb 89 b6 ed 50 18 dd 3c 7d d1 50
96 33 73 66 b9 88 fa d0 54 d8 e2 0d 68 a5 33 5d
8b f0 3f 23 32 78 c5 f3 66 a0 27 fe 0e 05 14 a3
d6 0a 35 88 e4 72 f0 7b 82 d2 d7 85 8c d7 c3 26

00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f

00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f
d6 aa 74 fd d2 af 72 fa da a6 78 f1 d6 ab 76 fe
b6 92 cf 0b 64 3d bd f1 be 9b c5 00 68 30 b3 fe
b6 ff 74 4e d2 c2 c9 bf 6c 59 0c bf 04 69 bf 41
47 f7 f7 bc 95 35 3e 03 f9 6c 32 bc fd 05 8d fd
3c aa a3 e8 a9 9f 9d eb 50 f3 af 57 ad f6 22 aa
5e 39 0f 7d f7 a6 92 96 a7 55 3d c1 0a a3 1f 6b
14 f9 70 1a e3 5f e2 8c 44 0a df 4d 4e a9 c0 26
47 43 87 35 a4 1c 65 b9 e0 16 ba f4 ae bf 7a d2
54 99 32 d1 f0 85 57 68 10 93 ed 9c be 2c 97 4e
13 11 1d 7f e3 94 4a 17 f3 07 a7 8b 4d 2b 30 c5

69 20 E2 99 A5 20 2A 41 74 6D 65 6C 41 56 52 2A

69 20 E2 99 A5 20 2A 41 74 6D 65 6C 41 56 52 2A
D9 20 07 1A 7C 00 2D 5B 08 6D 48 37 49 3B 1A 1D
39 82 A3 21 45 82 8E 7A 4D EF C6 4D 04 D4 DC 50
75 04 F0 D3 30 86 7E A9 7D 69 B8 E4 79 BD 64 B4
07 47 7D 65 37 C1 03 CC 4A A8 BB 28 33 15 DF 9C
4E D9 A3 A6 79 18 A0 6A 33 B0 1B 42 00 A5 C4 DE
68 C5 BE C5 11 DD 1E AF 22 6D 05 ED 22 C8 C1 33
C0 BD 7D 56 D1 60 63 F9 F3 0D 66 14 D1 C5 A7 27
E6 E1 B1 68 37 81 D2 91 C4 8C B4 85 15 49 13 A2
C6 9C 8B 31 F1 1D 59 A0 35 91 ED 25 20 D8 FE 87
91 27 9C 86 60 3A C5 26 55 AB 28 03 75 73 D6 84

*/