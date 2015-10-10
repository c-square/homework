#include <stdio.h>
#include <stdint.h>
#include "MD5.h"


using namespace std;
 
int main(int argc, char *argv[]) 
{
    uint8_t *input;
	uint8_t output[16];
    size_t inputLen;
	int i;

	char  tempInput[] = "password";
    
    if (argc < 2) 
	{
        printf("N-am primit argumente\n", argv[0]);
    }
	else
	{
		inputLen = strlen(tempInput);
		input = (uint8_t*) tempInput;
 
		//inputLen = strlen(tempInput);
		//input = (uint8_t*) tempInput;
 
		md5((uint8_t*)input, inputLen, output);
 
		printf("---------------------------\n");
		printf("Input : %s\n", input);
		printf("Output: ");
		for (i=0; i<16; i++)
			printf("%2x", output[i]);
		printf("\n---------------------------\n");

		scanf("%d", &i);
	}
 
    return 0;
}