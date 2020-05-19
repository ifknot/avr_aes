/*
 * aes_functions.cpp
 *
 * Created: 5/10/2020 5:27:27 PM
 *  Author: ifkno
 */ 

#include "aes_functions.h"

namespace avr_aes {
		
	void make_expanded_key(const key_t key, expanded_key_t xkey) {
		uint8_t j, k;
		uint8_t w[4]; // 32-bit Rijndael's word used for the column/row operations
		// The first round key is the key itself.
		for (uint8_t i{0}; i < N; ++i) {
			xkey[(i * 4) + 0] = key[(i * 4) + 0];
			xkey[(i * 4) + 1] = key[(i * 4) + 1];
			xkey[(i * 4) + 2] = key[(i * 4) + 2];
			xkey[(i * 4) + 3] = key[(i * 4) + 3];
		}
		// All other round keys are found from the previous round keys.
		for (uint8_t i{N}; i < XK / 4; ++i) {
			k = (i - 1) << 2u;
			w[0] = xkey[k + 0];
			w[1] = xkey[k + 1];
			w[2] = xkey[k + 2];
			w[3] = xkey[k + 3];
			if (i % N == 0) { //shift the 4 bytes in a word to the left once [a0,a1,a2,a3] becomes [a1,a2,a3,a0]
				const uint8_t rol = w[0];
				w[0] = w[1];
				w[1] = w[2];
				w[2] = w[3];
				w[3] = rol;
				//sbox mixing
				w[0] = pgm_read_byte(&(sbox[w[0]]));
				w[1] = pgm_read_byte(&(sbox[w[1]]));
				w[2] = pgm_read_byte(&(sbox[w[2]]));
				w[3] = pgm_read_byte(&(sbox[w[3]]));
				//Galois Field mix XOR round constant
				w[0] = w[0] ^ pgm_read_byte(&(Rcon[i / N]));
			}
			if (((R == R256) && i % N == 4)) { //extension for AES-256
				w[0] = pgm_read_byte(&(sbox[w[0]]));
				w[1] = pgm_read_byte(&(sbox[w[1]]));
				w[2] = pgm_read_byte(&(sbox[w[2]]));
				w[3] = pgm_read_byte(&(sbox[w[3]]));
			}
			//use the mixed word _w_ to XOR expand preceding key word into subsequent one
			j = i << 2u;
			k = (i - N) << 2u;
			xkey[j + 0] = xkey[k + 0] ^ w[0];
			xkey[j + 1] = xkey[k + 1] ^ w[1];
			xkey[j + 2] = xkey[k + 2] ^ w[2];
			xkey[j + 3] = xkey[k + 3] ^ w[3];
		}
	}

void add_round_key(uint8_t &rkey, const expanded_key_t xkey, uint8_t* i) {
		*(i + 0 ) ^= xkey[rkey++];
		*(i + 1 ) ^= xkey[rkey++];
		*(i + 2 ) ^= xkey[rkey++];
		*(i + 3 ) ^= xkey[rkey++];
		*(i + 4 ) ^= xkey[rkey++];
		*(i + 5 ) ^= xkey[rkey++];
		*(i + 6 ) ^= xkey[rkey++];
		*(i + 7 ) ^= xkey[rkey++];
		*(i + 8 ) ^= xkey[rkey++];
		*(i + 9 ) ^= xkey[rkey++];
		*(i + 10) ^= xkey[rkey++];
		*(i + 11) ^= xkey[rkey++];
		*(i + 12) ^= xkey[rkey++];
		*(i + 13) ^= xkey[rkey++];
		*(i + 14) ^= xkey[rkey++];
		*(i + 15) ^= xkey[rkey++];
	}

	void sub_bytes(uint8_t* i) {
		*(i + 0 ) = pgm_read_byte(&(sbox[*(i + 0 )]));
		*(i + 1 ) = pgm_read_byte(&(sbox[*(i + 1 )]));
		*(i + 2 ) = pgm_read_byte(&(sbox[*(i + 2 )]));
		*(i + 3 ) = pgm_read_byte(&(sbox[*(i + 3 )]));
		*(i + 4 ) = pgm_read_byte(&(sbox[*(i + 4 )]));
		*(i + 5 ) = pgm_read_byte(&(sbox[*(i + 5 )]));
		*(i + 6 ) = pgm_read_byte(&(sbox[*(i + 6 )]));
		*(i + 7 ) = pgm_read_byte(&(sbox[*(i + 7 )]));
		*(i + 8 ) = pgm_read_byte(&(sbox[*(i + 8 )]));
		*(i + 9 ) = pgm_read_byte(&(sbox[*(i + 9 )]));
		*(i + 10) = pgm_read_byte(&(sbox[*(i + 10)]));
		*(i + 11) = pgm_read_byte(&(sbox[*(i + 11)]));
		*(i + 12) = pgm_read_byte(&(sbox[*(i + 12)]));
		*(i + 13) = pgm_read_byte(&(sbox[*(i + 13)]));
		*(i + 14) = pgm_read_byte(&(sbox[*(i + 14)]));
		*(i + 15) = pgm_read_byte(&(sbox[*(i + 15)]));
	}

	void shift_rows(uint8_t* i) {
		// Rotate first row 1 columns to left
		uint8_t rol{*(i + 1 )};
		*(i + 1 ) = *(i + 5 );
		*(i + 5 ) = *(i + 9 );
		*(i + 9 ) = *(i + 13);
		*(i + 13) = rol;
		// Rotate second row 2 columns to left
		rol = *(i + 2 );
		*(i + 2 ) = *(i + 10);
		*(i + 10) = rol;
		rol = *(i + 6 );
		*(i + 6 ) = *(i + 14);
		*(i + 14) = rol;
		// Rotate third row 3 columns to left
		rol = *(i + 3 );
		*(i + 3 ) = *(i + 15);
		*(i + 15) = *(i + 11);
		*(i + 11) = *(i + 7 );
		*(i + 7 ) = rol;
	}

	void mix_columns(uint8_t* i) {
		uint8_t a, b, c;
		//first column
		a = *(i + 0);
		b = *(i + 0) ^ *(i + 1);
		c = *(i + 0) ^ *(i + 1) ^ *(i + 2) ^ *(i + 3);
		b = GF2(b);
		*(i + 0) ^= b ^ c;
		b = *(i + 1) ^ *(i + 2);
		b = GF2(b);
		*(i + 1) ^= b ^ c;
		b = *(i + 2) ^ *(i + 3);
		b = GF2(b);
		*(i + 2) ^= b ^ c;
		b = *(i + 3) ^ a;
		b = GF2(b);
		*(i + 3) ^= b ^ c;
		//second column
		a = *(i + 4);
		c = *(i + 4) ^ *(i + 5) ^ *(i + 6) ^ *(i + 7);
		b = *(i + 4) ^ *(i + 5);
		b = GF2(b);
		*(i + 4) ^= b ^ c;
		b = *(i + 5) ^ *(i + 6);
		b = GF2(b);
		*(i + 5) ^= b ^ c;
		b = *(i + 6) ^ *(i + 7);
		b = GF2(b);
		*(i + 6) ^= b ^ c;
		b = *(i + 7) ^ a;
		b = GF2(b);
		*(i + 7) ^= b ^ c;
		//third column
		a = *(i + 8);
		c = *(i + 8) ^ *(i + 9) ^ *(i + 10) ^ *(i + 11);
		b = *(i + 8) ^ *(i + 9);
		b = GF2(b);
		*(i + 8) ^= b ^ c;
		b = *(i + 9) ^ *(i + 10);
		b = GF2(b);
		*(i + 9) ^= b ^ c;
		b = *(i + 10) ^ *(i + 11);
		b = GF2(b);
		*(i + 10) ^= b ^ c;
		b = *(i + 11) ^ a;
		b = GF2(b);
		*(i + 11) ^= b ^ c;
		//fourth column
		a = *(i + 12);
		c = *(i + 12) ^ *(i + 13) ^ *(i + 14) ^ *(i + 15);
		b = *(i + 12) ^ *(i + 13);
		b = GF2(b);
		*(i + 12) ^= b ^ c;
		b = *(i + 13) ^ *(i + 14);
		b = GF2(b);
		*(i + 13) ^= b ^ c;
		b = *(i + 14) ^ *(i + 15);
		b = GF2(b);
		*(i + 14) ^= b ^ c;
		b = *(i + 15) ^ a;
		b = GF2(b);
		*(i + 15) ^= b ^ c;
	}

	void encrypt(const expanded_key_t ekey, uint8_t* i) {
		uint8_t rkey{0};				//offset in to the expanded keystruct
		// xor the first round key to the block before starting the rounds
		add_round_key(rkey, ekey, i);	//increments the rkey offset iterator _i_ + 16
		// loop through the remaining rounds until penultimate round
		for(auto j{ R - 1 }; --j;) {	// the R - 1 rounds are identical...
			sub_bytes(i);
			shift_rows(i);				// Rijndael diffusion
			mix_columns(i);				// Rijndael diffusion
			add_round_key(rkey, ekey, i);
		}
		// final round lacks mix_columns diffusion
		sub_bytes(i);
		shift_rows(i);
		add_round_key(rkey, ekey, i);
	}

}

/*

mix columns test vectors:
Before			After
db 13 53 45		8e 4d a1 bc
f2 0a 22 5c		9f dc 58 9d
01 01 01 01		01 01 01 01
c6 c6 c6 c6		c6 c6 c6 c6
d4 d4 d4 d5		d5 d5 d7 d6

*/
