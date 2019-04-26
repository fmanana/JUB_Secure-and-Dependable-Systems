#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scrypt.h"

int main(int argc, char **argv)
{
    unsigned char *m = (unsigned char*) "secret";
    size_t len = strlen((char*) m);
    uint32_t key = 0x98267351;
    unsigned char cypher[len], decode[len];

    printf("Question b): Electronic Codebook Encryption:\n");
    printf("Key: 0x%x\n\n", key);
    printf("Clear text: %s (0x", m);
    for(int i = 0; i < len; i++)
        printf("%x", m[i]);
    printf(")\n");

    // EC encryption
    sc_enc_ecb(m, cypher, len, key);

    printf("Encrypted text: %s (0x", cypher);
    for(int i = 0; i < len; i++)
        printf("%x", cypher[i]);
    printf(")\n");

    // EC decryption
    sc_dec_ecb(cypher, decode, len, key);

    printf("Decrypted text: %s (0x", decode);
    for(int i = 0; i < len; i++)
        printf("%x", decode[i]);
    printf(")\n-----------------------------\n");



	unsigned char *n = (unsigned char*) "hacker";
	len = strlen((char*) n);
	unsigned char cypher2[len], decode2[len];
    uint8_t iv = 0x42;

    printf("Question c): Cypher Block Chaining Encryption:\n");
    printf("Key: 0x%x\n\n", key);
    printf("Clear text: %s (0x", n);
    for(int i = 0; i < len; i++)
        printf("%x", n[i]);
    printf(")\n");

    // CBC encryption
	sc_enc_cbc(n, cypher2, len, key, iv);

    printf("Encrypted text: %s (0x", cypher2);
    for(int i = 0; i < len; i++)
        printf("%x", cypher2[i]);
    printf(")\n");

    // CBC decryption
	sc_dec_cbc(cypher2, decode2, len, key, iv);
	
    printf("Decrypted text: %s (0x", decode2);
    for(int i = 0; i < len; i++)
        printf("%x", decode2[i]);
    printf(")\n------------------------------\n");
    



    unsigned char cypherBlocks[] = {0xc6, 0x5e, 0x05, 0x94, 0x6b, 0x86, 0xeb, 0x2e, 0x33, 0xf5,0x8f, 0xda, 0xff, 0x0f, 0x42};
    len = sizeof(cypherBlocks);
    unsigned char decode3[len];

    printf("Question c): Cypher Block Chaining Decryption:\n");
    printf("Key: 0x%x\n\n", key);
    printf("Encrypted text: ");
    for(int i = 0; i < len; i++)
        printf("%c", (char)cypherBlocks[i]);
    
    printf(" (0xc65e05946b86eb2e33f58fdaff0f42)\n");
    
    // CBC decryption
    sc_dec_cbc(cypherBlocks, decode3, len, key, iv);
    
    printf("Decrypted text: %s (0x", decode3);
    for(int i = 0; i < len; i++)
    {
        printf("%x", decode3[i]);
    }

	printf(")\n-----------------------------\n");

    return EXIT_SUCCESS;
}   