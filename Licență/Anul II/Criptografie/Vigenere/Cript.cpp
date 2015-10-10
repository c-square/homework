#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include "Vigenere.h"
#include "Common.h"

using namespace std;

ViginereCD viginere;
string tempBuff;
string tempKey;

void commander()
{
	int command;
	bool ok = true;
	printHeader();
	printHelp();

	ofstream fout("buffer.txt");

	while (ok)
	{
		command = takeCommand();
		switch (command)
		{
			case 1:
				printf("\t->Filename please:");
				cin>>tempBuff;

				if(viginere.OpenFile(tempBuff))
					printf("\t->File has been loaded. Buffer set\n");
				else
					printf("\t->Problem loading file. Maybe it doesn't exist\n");

				break;
			case 2:

				printf("\t->Buffer please:");
				cin>>tempBuff;

				if(viginere.SetBuffer(tempBuff))
					printf("\t->Buffer has been loaded\n");
				else
					printf("\t->Problem loading buffer\n");

				break;
			case 3:

				printf("\t->Key please:");
				cin>>tempKey;

				if(viginere.SetKey(tempKey))
					printf("\t->Key has been loaded\n");
				else
					printf("\t->Problem loading key\n");

				break;
			case 4:

				printf("\t->Crypting...\n");
				if (viginere.CryptBuffer())
					printf("\t->Buffer Crypted :)\n");
				else
					printf("\t->Problems crypting text\n");


				break;

			case 5:

				printf("\t->Outputing buffer to buffer.txt\n");
				
				cout<<"\t->Buffer: "<<viginere.GetBuffer()<<" \n";
				fout<<viginere.GetBuffer();

				break;

			case 6:

				cout<<"\t->Key: "<<viginere.GetKey()<<" \n";

				break;
			//decrypt
			case 7:

				printf("\t->Decrypting...\n");
				if (viginere.DecryptBuffer())
					printf("\t->Buffer decrypted\n\n");
				else
					printf("\t->FAIL! \n");

				break;

			case 8:

				printf("\n\n ;;; exiting ;;; \n");
				ok = false;
				break;

			default:

				printf("\n\nWrong command\n\n");
				break;
		}
	}

	fout.close();
}


int main ()
{
	commander();

	return 0;
}