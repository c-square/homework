#include "common.h"
#include <string.h>
#include <bitset>

using namespace std;


void printHeader()
{
	printf("-----------------------------------\n");
	printf("-\t\tDES Crypter/Decrypter\t-\n");
	printf("-----------------------------------\n");
}

void printHelp()
{
	printf("\n\n");
	printf("1. Load plaintext \n");
	printf("2. Load cryptotext \n");
	printf("3. Input plaintext \n");
	printf("4. Input cryptotext \n");
	printf("5. Input key \n");
	printf("6. Crypt\n");
	printf("7. Decrypt\n");
	printf("8. Output plaintext\n");
	printf("9. Output cryptotext\n");
	printf("10. Output key\n");
	printf("11. Try to decrypt the cool way \n");
	printf("12. Quit \n");
	printf("----------------------------------\n\n");
}

int takeCommand()
{
	int nr;

	while (1)
	{
		printf("--> ");
		scanf("%d", &nr);

		if (nr <= nrCommands)
			return nr;
		else
			printf ("\nWrong Command\n");
	}
}

int isWhiteChar(char c)
{
	return !isalpha(c);
}

bool isMyChar(char c)
{
	if ((c>='a' && c<='z') || (c>='A' && c<='Z'))
	{
		return true;
	}
	else
		return false;
}


bool readYesNo()
{
	char c = 0;
	while (c!='y' && c!='n' && c!='Y' && c!='N')
	{
		printf("\n->Raspuns: ");
		cin>>c;
	}

	if (c == 'y' || c== 'Y')
		return 1;
	else
		return 0;
}







