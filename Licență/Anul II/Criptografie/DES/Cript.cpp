#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include "DES.h"
#include "Common.h"

using namespace std;

DES_CD des;
string tempBuff;
string tempKey;

void commander()
{
	int command;
	bool ok = true;
	printHeader();
	printHelp();

	ofstream plainFout("plain.txt");
	ofstream cryptoFout("crypto.txt");

	while (ok)
	{
		command = takeCommand();
		switch (command)
		{
			case 1:
				printf("\t->PlainText Filename please:");
				cin>>tempBuff;

				if(des.OpenFile(tempBuff, true))
					printf("\t->File has been loaded. PlainText set\n");
				else
					printf("\t->Problem loading file. Maybe it doesn't exist\n");

				break;
			case 2:
				printf("\t->CryptoText Filename please:");
				cin>>tempBuff;

				if(des.OpenFile(tempBuff, false))
					printf("\t->File has been loaded. CryptoText set\n");
				else
					printf("\t->Problem loading file. Maybe it doesn't exist\n");

				break;
			case 3:

				printf("\t->PlainText please:");
				cin>>tempBuff;

				if(des.SetPlaintext(tempBuff))
					printf("\t->PlainText has been loaded\n");
				else
					printf("\t->Problem loading buffer\n");

				break;
			case 4:

				printf("\t->CryptoText please:");
				cin>>tempBuff;

				if(des.SetCryptotext(tempBuff))
					printf("\t->CryptoText has been loaded\n");
				else
					printf("\t->Problem loading buffer\n");

				break;
			case 5:

				printf("\t->Key please:");

				cin>>tempKey;

				if (tempKey.size()!=8 )
				{
					while (tempKey.size()!=8)
					{
						printf("\t->Key was of invalid lenght (must be 8). Please input key: ");
						cin>>tempKey;
					}
				}

				if(des.SetKey(tempKey))
					printf("\t->Key has been loaded\n");
				else
					printf("\t->Problem loading key\n");

				break;
			case 6:

				printf("\t->Crypting...\n");
				if (des.Crypt())
					printf("\t->Plaintext Crypted :)\n");
				else
					printf("\t->Problems crypting plaintext\n");


				break;
			case 7:

				printf("\t->Decrypting...\n");
				if (des.Decrypt())
					printf("\t->Cryptotext decrypted :)\n");
				else
					printf("\t->Problems decrypting text\n");


				break;

			case 8:

				printf("\t->Outputing plainText to plaintext.txt\n");
				
				//cout<<"\t->Plaintext: "<<des.GetPlaintext()<<" \n";
				plainFout<<des.GetPlaintext();

				break;

			case 9:

				printf("\t->Outputing cryptoText to cryptotext.txt\n");
				
				//cout<<"\t->cryptotext: "<<des.GetCryptotext()<<" \n";
				cryptoFout<<des.GetCryptotext();

				break;

			case 10:

				cout<<"\t->Key: "<<des.GetKey()<<" \n";

				break;
			//decrypt
			case 11:
				printf("\t->Hack Decrypting...\n");
				if (des.Attack("inputAttack.txt"))
					printf("\t->Attack ok!\n\n");
				else
					printf("\t->Attack not ok! \n");

				break;

			case 12:

				printf("\n\n ;;; exiting ;;; \n");
				ok = false;
				break;

			default:

				printf("\n\nWrong command\n\n");
				break;
		}
	}

	plainFout.close();
	cryptoFout.close();
}


int main ()
{
	commander();

	return 0;
}