#include <string.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cctype>
#include <vector>
#include <math.h>
#include "Common.h"
#include "Vigenere.h"

using namespace std;


	ViginereCD::ViginereCD(){}
	ViginereCD::ViginereCD(string buff)
	{
		this->SetBuffer(buff);
		this->bufferSet = true;
	}
	ViginereCD::ViginereCD(const char *filePath)
	{
		OpenFile(filePath);
	}

	bool ViginereCD::LoadFile(ifstream &fin)
	{
		int i;
		
		fin.seekg(0, std::ios::end);
		this->buffer.resize(fin.tellg());
		fin.seekg(0, std::ios::beg);
		fin.read(&(this->buffer)[0], this->buffer.size());

		//scot caracterele albe
		for (i=0; i<this->buffer.size(); )
		{
			if (!isMyChar(this->buffer[i]))
				this->buffer.erase(this->buffer.begin() + i);
			else
				i++;
		}

		//tin minte ca mi-am setat buffer-ul
		this->bufferSet = true;

		return true;
	}

	bool ViginereCD::OpenFile(string filename)
	{
		ifstream fin(filename.data(), ifstream::in);

		if (this->LoadFile (fin))
		{
			fin.close();
			return true;
		}
		return false;
	}

	bool ViginereCD::OpenFile(const char * filename)
	{
		ifstream fin(filename, ifstream::in);

		if (this->LoadFile (fin))
		{
			fin.close();
			return true;
		}
		return false;
	}




	bool ViginereCD::CryptBuffer()
	{
		int i, j;

		if (this->keySet && this->bufferSet)
		{
			for (i=0, j=0; i<this->buffer.size(); i++)
			{
				//criptez
				buffer[i] = toupper(buffer[i]);
				buffer[i] = (((buffer[i] + toupper(key[j]) - (65) + 1 ) - 65) %26) + 65;

				//actualizez index-ul pentru cheie
				j++;
				j %= key.size();
			}
		}
		else
			return false;
		return true;
	}


	bool ViginereCD::DecryptBuffer()
	{
		int keyLen;
		int i, j;

		if (bufferSet)
		{
			keyLen = detKeyLenght();
			printf("\n:: The key has been set: %d\n", keyLen);
			detKey(keyLen);
			
			/*
			for (i=0, j=0; i<this->buffer.size(); i++)
			{
				//criptez
				buffer[i] = toupper(buffer[i]);
				buffer[i] = (buffer[i] - 65) - (toupper(key[j]) - 65);k
				
				if (buffer[i]<0) buffer[i]+=25; 

				buffer[i]+=65;

				//actualizez index-ul pentru cheie
				j++;
				j %= key.size();
			}
			*/
		}
		else
		{
			printf("\n\n:: The buffer has not been set \n\n");
		}

		return true;
	}

	bool ViginereCD::SetBuffer(string buff)
	{
		this->buffer = buff;
		this->bufferSet = true;
		return true;
	}

	bool ViginereCD::SetKey(string key)
	{
		this->key = key;
		this->keySet = true;
		return true;
	}

	bool ViginereCD::UnsetKey()
	{
		this->key = "";
		this->keySet = false;
		return true;
	}

	string ViginereCD::GetBuffer()
	{
		return this->buffer;
	}

	string ViginereCD::GetKey()
	{
		return this->key;
	}


	int ViginereCD::detKeyLenght()
	{
		vector<double> ICValues[MaxKeyLen+2];
		int i, j, selectedKeyLen = -1;
		double bestIC, crtIC;
		bool response;

		// Calculez pentru fiecare lungime de cheie setul de Indici de Coincidenta
		for (i=1; i<=MaxKeyLen; i++)
		{
			for (j=0; j<i; j++)
			{
				ICValues[i].insert(ICValues[i].end(), IndiceCoincidenta(i, j));
			}
		}

		// Caut cel mai bun set (media indicilor sa fie cat mai apropiata de 0.065)
		for (i=1, bestIC=100.0; i<=MaxKeyLen; i++)
		{
			crtIC = 0;
			for (j=0; j<ICValues[i].size(); j++)
			{
				crtIC += abs(ICValues[i][j] - 0.065); 
			}
			crtIC = crtIC / ICValues[i].size();


			if (crtIC < bestIC)
			{
				bestIC = crtIC;
				selectedKeyLen = i;
			}
		}


		// Afisez toate datele, si ofer user-ului posibilitatea de a corecta lungimea cheii (in caz de valori aberante)
		printf("\n\n:: IC values and selected key lenght:\n");
		printf("\nKey lenght: %d\n\n", selectedKeyLen);
		for (i=1; i<=MaxKeyLen; i++)
		{
			printf("%d . ", i);
			for (j=0; j<ICValues[i].size(); j++)
			{
				printf("%.3f ", ICValues[i][j]);
			}
			printf("\n");
		}

		printf(":: Do you wish to choose another key lenght? (y/n) \n");
		response = readYesNo();

		if (response)
		{
			printf("\n -> New Key lenght: ");
			cin>>selectedKeyLen;
		}

		return selectedKeyLen;
	}

	double ViginereCD::IndiceCoincidenta(int posKeyLen, int start)
	{
		int frecv[30];
		int i;
		double sum = 0.0;
		double buffSize = buffer.size();
		double len = 0; 

		for (i=0; i<=29; i++)
			frecv[i] = 0;

		for (i=start; i<buffSize; i+=posKeyLen)
		{
			frecv[this->buffer[i]-65]++;
			len++;
		}


		for (i=0; i<26; i++)
			sum += 1.0*((  frecv[i]*(frecv[i]-1)  ) / ( len * (len - 1)  ));

		return sum;
	}


	bool ViginereCD::detKey(int keyLen)
	{
		vector<double> frecv[MaxKeyLen];
		string key;

		int i, j, lg;

		// Determin frecventele literelor din keyLen in keyLen
		for (i=0; i<keyLen; i++)
		{
			lg = 0;
			//initializare
			for (j=0; j<=26; j++)
				frecv[i].insert(frecv[i].end(), 0);

			//count
			for (j=i; j<buffer.size(); j+=keyLen)
			{
				frecv[i][buffer[j]-65]++;
				lg++;
			}

			//impartire
			for (j=0; j<=26; j++)
				frecv[i][j]=(1.0*frecv[i][j])/lg;			
		}

		for (i=0; i<keyLen; i++)
		{
			key.insert(key.end(),  MIC(&frecv[i]));
		}

		this->key = key;

		cout<<"Key: \n"<<key;

		return true;
	}

	char ViginereCD::MIC(vector<double> * frecv)
	{
		double frecvStd[26] = {0.082, 0.015, 0.028, 0.043, 0.0127, 0.022, 0.02, 0.061, 0.07, 0.002, 0.08, 0.04, 0.024, 0.067, 0.075, 0.019, 0.001, 0.06, 0.063, 0.091, 0.028, 0.01, 0.023, 0.001, 0.02, 0.001};
		//double frecvStd[26] = {7.81, 1.28, 2.93, 4.11, 13.05, 2.88, 1.39, 5.85, 6.77, 0.23, 0.42, 3.60, 2.62, 7.28, 8.21, 2.15, 0.14, 6.64, 6.46, 9.02, 2.77, 1.00, 1.49, 0.30, 1.51, 0.09};
		int i, j, litera;
		double sum, diffMin = 100, temp;

		for (i=0; i<26; i++)
		{
			sum = 0;
			//calcul suma pe litera curenta (i)
			for (j=0; j<26; j++)
				//sum += ((*frecv)[j] * (1.0* frecvStd[j] / 100));
				sum += ((*frecv)[j] * (1.0* frecvStd[j]));

			if ( abs (sum - 0.065) < diffMin)
			{
				diffMin = abs(sum - 0.065);
				litera = i;
			}
			//shift
			temp = frecvStd[25];
			for (j=25; j>0; j--)
				frecvStd[j] = frecvStd[j-1];
			frecvStd[0] = temp;
		}

		if (litera-1<0)
			litera = 26;
		return  (litera-1)+65;
	}

