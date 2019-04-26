//Notice The following code was worked on with Brian Sherif and Dragi Kamov
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "scrypt.h"

void Substitution(uint8_t *m)
{
    // high and lo bits of m
    uint8_t hi, lo;

    hi = *m >> 4;
    hi = ((hi + 1) * 7) % (17 - 1);

    lo = *m << 4;
    lo = lo >> 4;
    lo = ((lo + 1) * 7) % (17 - 1);

    *m = (hi << 4) | lo;
}

void InvSubstitution(uint8_t *c)
{
    for(int i = 0; i < 3; i++)
        Substitution(c);
}


void Permutation(uint8_t *m)
{
    *m = (*m << 2) | (*m >> 6);
}

void InvPermutation(uint8_t *c)
{
    for(int i = 0; i < 3; i++)
        Permutation(c);
}

// 4 round block
uint8_t sc_enc8(uint8_t m, uint32_t k)
{
    uint8_t cypher;
    uint8_t keys[4];

    // split key into 8 bit segments
    for(int i = 0; i < 4; i++)
        keys[3 - i] = k >> (8 * i);

    // Round 0
    cypher = m ^ keys[0];

    // Round 1
    Substitution(&cypher);
    Permutation(&cypher);
    cypher = cypher ^ keys[1];

    // Round 2
    Substitution(&cypher);
    Permutation(&cypher);
    cypher = cypher ^ keys[2];

    // Round 3
    Substitution(&cypher);
    cypher = cypher ^ keys[3];

    return cypher;
}

uint8_t sc_dec8(uint8_t c, uint32_t k)
{
    uint8_t cleartext;
    uint8_t keys[4];

    // split key into 8 bit segments
    for(int i = 0; i < 4; i++)
        keys[3 - i] = k >> (8 * i);
    
    // Round 0
    cleartext = c ^ keys[3];
    InvSubstitution(&cleartext);

    // Round 1
    cleartext = cleartext ^ keys[2];
    InvPermutation(&cleartext);
    InvSubstitution(&cleartext);

    // Round 2
    cleartext = cleartext ^ keys[1];
    InvPermutation(&cleartext);
    InvSubstitution(&cleartext);

    // Round 3
    cleartext = cleartext ^ keys[0];

    return cleartext;
}


void sc_enc_ecb(unsigned char *m, unsigned char *c, size_t len, uint32_t k)
{
    uint8_t blocks[len];
    for(int i = 0; i < len; i++)
        blocks[i] = m[i];
    
    for(int i = 0; i < len; i++)
        c[i] = sc_enc8(blocks[i], k);
}

void sc_dec_ecb(unsigned char *c, unsigned char *m, size_t len, uint32_t k)
{
    uint8_t blocks[len];
    for(int i = 0; i < len; i++)
        blocks[i] = c[i];
    
    for(int i = 0; i < len; i++)
        m[i] = sc_dec8(blocks[i], k);
}

void sc_enc_cbc(unsigned char *m, unsigned char *c, size_t len, uint32_t k, uint8_t iv)
{
    uint8_t blocks[len];
    for(int i = 0; i < len; i++)
        blocks[i] = m[i];
	
	// first block
    c[0] = sc_enc8(blocks[0] ^ iv, k);

    for(int i = 1; i < len; i++)
		c[i] = sc_enc8(c[i-1] ^ blocks[i], k);
}

void sc_dec_cbc(unsigned char *c, unsigned char *m, size_t len, uint32_t k, uint8_t iv)
{
	uint8_t blocks[len];
	for(int i = 0; i < len; i++)
    	blocks[i] = c[i];

	// first block
	m[0] = sc_dec8(blocks[0], k) ^ iv;

	for(int i = 1; i < len; i++)
		m[i] = blocks[i-1] ^ sc_dec8(blocks[i], k);
}