#include "common.h"
#include <string.h>


void printHeader()
{
	printf("-----------------------------------\n");
	printf("-\t\tViginere Crypter\Decrypter\t-\n");
	printf("-----------------------------------\n");
}

void printHelp()
{
	printf("\n\n");
	printf("1. Load a file \n");
	printf("2. Input a buffer \n");
	printf("3. Input key \n");
	printf("4. Crypt\n");
	printf("5. Output buffer\n");
	printf("6. Key?\n");
	printf("7. Try to decrypt the cool way \n");
	printf("8. Quit \n");
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