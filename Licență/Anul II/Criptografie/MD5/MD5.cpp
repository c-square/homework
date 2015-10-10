#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "MD5.h"

void intToCharBytes(uint32_t val, uint8_t *buffer)
{
    buffer[0] = (uint8_t) val;
    buffer[1] = (uint8_t) (val >> 8);
    buffer[2] = (uint8_t) (val >> 16);
    buffer[3] = (uint8_t) (val >> 24);
}
 
uint32_t charToIntBytes(const uint8_t *buffer)
{
    return (uint32_t) buffer[0] | ((uint32_t) buffer[1] << 8) | ((uint32_t) buffer[2] << 16) | ((uint32_t) buffer[3] << 24);
}
 
void md5(const uint8_t *input, int inputLen, uint8_t *output) 
{
    uint32_t p0, p1, p2, p3;
    uint8_t *extendedInput = NULL;
 
    int extendedLen, sectiune;
    uint32_t chunks[16];
    uint32_t a, b, c, d, i, f, g, temp;
 
    // Hash-ul initial
    p0 = 0x67452301;
    p1 = 0xefcdab89;
    p2 = 0x98badcfe;
    p3 = 0x10325476;
 
    /////// EXTINDERE
	extendInput(input, inputLen, &extendedInput, &extendedLen);
 
    // pentru fiecare bucata de 512 biti
    for(sectiune=0; sectiune<extendedLen; sectiune += (512/8)) {
 
		// spargem in 16 bucati de cate 32 de biti
        for (i = 0; i < 16; i++)
            chunks[i] = charToIntBytes(extendedInput + sectiune + i*4);
 
        a = p0;
        b = p1;
        c = p2;
        d = p3;
 
       
        for(i = 0; i<64; i++)
		{
            if (i < 16) 
			{
                f = (b & c) | ((~b) & d);
                g = i;
            } 
			else 
			if (i < 32) 
			{
                f = (d & b) | ((~d) & c);
                g = (5*i + 1) % 16;
            } 
			else 
			if (i < 48) 
			{
                f = b ^ c ^ d;
                g = (3*i + 5) % 16;          
            }
			else 
			{
                f = c ^ (b | (~d));
                g = (7*i) % 16;
            }
 
            temp = d;
            d = c;
            c = b;
            b = b + ROL((a + f + constants[i] + chunks[g]), shifts[i]);
            a = temp;
 
        }
 
        // Adaugam hash-ul pentru bucata asta de 512 biti la hash-ul final
        p0 += a;
        p1 += b;
        p2 += c;
        p3 += d;
    }
 
    free(extendedInput);
 
    // salvare hash in buffer-ul de output
    intToCharBytes(p0, output);
    intToCharBytes(p1, output + 4);
    intToCharBytes(p2, output + 8);
    intToCharBytes(p3, output + 12);
}
 


void extendInput(const uint8_t * input, int inputLen, uint8_t ** extendedInput, int * extendedLen)
{
	int i;
	for ((*extendedLen) = inputLen + 1; (*extendedLen) % (512/8) != 448/8; (*extendedLen)++);
 
    (*extendedInput) = (uint8_t*)malloc((*extendedLen) + 8);
    memcpy((*extendedInput), input, inputLen);

    (*extendedInput)[inputLen] = 0x80; // pune primii 32 de biti dupa buffer (pentru extensie) = 100000....
    for (i = inputLen + 1; i < (*extendedLen); i++)
        (*extendedInput)[i] = 0; //extindem cu 0 pana la (multiplu de 512) - 64 biti
 
    //adaugam la sfarsit lungimea originala a mesajului
    intToCharBytes(inputLen*8, (*extendedInput) + (*extendedLen));
    intToCharBytes(inputLen>>29, (*extendedInput) + (*extendedLen) + 4);
}
