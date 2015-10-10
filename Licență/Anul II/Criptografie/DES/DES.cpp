#include <string.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cctype>
#include <vector>
#include <math.h>
#include <bitset>
#include "Common.h"
#include "DES.h"

using namespace std;


DES_CD::DES_CD(){}

bool DES_CD::LoadFile(ifstream &fin, bool ifPlain)
{
	int i;
	string * buff;
	if (ifPlain)
		buff = &this->plainText;
	else
		buff = &this->cryptoText;
		
	fin.seekg(0, std::ios::end);
	(*buff).resize(fin.tellg());
	fin.seekg(0, std::ios::beg);
	fin.read(&(*buff)[0], (*buff).size());

	//scot caracterele albe
	for (i=0; i<(*buff).size(); )
	{
		if (!isMyChar((*buff)[i]))
			(*buff).erase((*buff).begin() + i);
		else
			i++;
	}

	//tin minte ca mi-am setat buffer-ul
	if (ifPlain)
		this->plainSet = true;
	else
		this->cryptoSet = true;

	return true;
}

bool DES_CD::OpenFile(string filename, bool ifPlain)
{
	ifstream fin(filename.data(), ifstream::in);

	if (this->LoadFile (fin, ifPlain))
	{
		fin.close();
		return true;
	}
	fin.close();
	return false;
}

bool DES_CD::OpenFile(const char * filename, bool ifPlain)
{
	ifstream fin(filename, ifstream::in);

	if (this->LoadFile (fin, ifPlain))
	{
		fin.close();
		return true;
	}
	return false;
}


bool DES_CD::SetPlaintext(string buff)
{
	this->plainText = buff;
	this->plainSet = true;
	return true;
}

bool DES_CD::SetCryptotext(string buff)
{
	this->cryptoText = buff;
	this->cryptoSet = true;
	return true;
}

bool DES_CD::SetKey(string key)
{
	string temp = "";
	int i, j, k;

	this->key = key;
	this->keySet = true;

	stringToBitset(this->key, &(this->bitKey));

	this->generateKeys();

	return true;
}

bool DES_CD::UnsetKey()
{
	this->key = "";
	this->keySet = false;
	return true;
}

string DES_CD::GetPlaintext()
{
	return this->plainText;
}

string DES_CD::GetCryptotext()
{
	return this->cryptoText;
}

string DES_CD::GetKey()
{
	return this->key;
}


void DES_CD::generateKeys()
{
	int i, j;
	bitset<56> strippedKey, permutedKey, temp;
	bitset<48> crtKey;
	bitset<28> CCrt, DCrt;

	//am scos bitii in plus
	/*for (i=1, j=0; i<=64; i++)
		if (i%8 != 0)
		{
			strippedKey[j++] = this->bitKey[i-1];
		}
	*/

	//permutam
	/*
	for (i=0; i<56; i++)
	{
		permutedKey[i] = strippedKey[PC1[i]-1];
	}
	*/
	for (i=0; i<56; i++)
	{
		permutedKey[i] = this->bitKey[PC1[i]-1];
	}


	splitBitsetInHalf56(&permutedKey, &CCrt, &DCrt);

	//generarea tuturor cheilor
	for (i=1; i<=NrRunde; i++)
	{
		if (i==1 || i==2 || i==9 || i==16)
		{	
			rotateLeft(CCrt, 1);
			rotateLeft(DCrt, 1);
		}
		else
		{
			rotateLeft(CCrt, 2);
			rotateLeft(DCrt, 2);
		}

		//unirea
		joinBitsetFromHalfs56(&temp, &CCrt, &DCrt);

		//permutarea
		for (j=0; j<48; j++)
			crtKey[j] = temp[PC2[j]-1];

		switch(i)
		{
		case 1: K1 = crtKey; break;
		case 2: K2 = crtKey; break;
		case 3: K3 = crtKey; break;
		case 4: K4 = crtKey; break;
		case 5: K5 = crtKey; break;
		case 6: K6 = crtKey; break;
		case 7: K7 = crtKey; break;
		case 8: K8 = crtKey; break;
		case 9: K9 = crtKey; break;
		case 10: K10 = crtKey; break;
		case 11: K11 = crtKey; break;
		case 12: K12 = crtKey; break;
		case 13: K13 = crtKey; break;
		case 14: K14 = crtKey; break;
		case 15: K15 = crtKey; break;
		case 16: K16 = crtKey; break;
		}
	}
}


void splitBitsetInHalf(bitset<64> * src, bitset<32> * L0, bitset<32> * R0)
{
	int i;
	for (i=0; i<32; i++)
	{
		(*R0)[i] = (*src)[i];
	}

	for (i=32; i<64; i++)
	{
		(*L0)[i-32] = (*src)[i];
	}
}

void splitBitsetInHalf56(bitset<56> * src, bitset<28> * L0, bitset<28> * R0)
{
	int i;
	for (i=0; i<28; i++)
	{
		(*R0)[i] = (*src)[i];
	}

	for (i=28; i<56; i++)
	{
		(*L0)[i-28] = (*src)[i];
	}
}

void joinBitsetFromHalfs(bitset<64> * dest, bitset<32> * L16, bitset<32> * R16)
{
	int i;
	
	for (i=0; i<32; i++)
			(*dest)[i] = (*R16)[i];
	for (i=32; i<64; i++)
			(*dest)[i] = (*L16)[i-32];
}

void joinBitsetFromHalfs56(bitset<56> * dest, bitset<28> * L16, bitset<28> * R16)
{
	int i;
	
	for (i=0; i<28; i++)
			(*dest)[i] = (*R16)[i];
	for (i=28; i<56; i++)
			(*dest)[i] = (*L16)[i-28];
}

template <std::size_t N> void rotateLeft(std::bitset<N>& b, unsigned m)
{
b = b << m | b >> (N-m);
}


void getCrtKey(bitset<48> * key, int poz)
{
	int i;

	switch (poz)
	{
	case 1: (*key) = K1; break;
	case 2: (*key) = K2; break;
	case 3: (*key) = K3; break;
	case 4: (*key) = K4; break;
	case 5: (*key) = K5; break;
	case 6: (*key) = K6; break;
	case 7: (*key) = K7; break;
	case 8: (*key) = K8; break;
	case 9: (*key) = K9; break;
	case 10: (*key) = K10; break;
	case 11: (*key) = K11; break;
	case 12: (*key) = K12; break;
	case 13: (*key) = K13; break;
	case 14: (*key) = K14; break;
	case 15: (*key) = K15; break;
	case 16: (*key) = K16; break;
	}
}


void bitsetXOR6(bitset<6> * dest, bitset<6> * p1, bitset<6> *p2)
{
	int i;

	for (i=0; i<6; i++)
	{
		(*dest)[i] = (*p1)[i] ^ (*p2)[i];
	}
}

void bitsetXOR4(bitset<4> * dest, bitset<4> * p1, bitset<4> *p2)
{
	int i;

	for (i=0; i<4; i++)
	{
		(*dest)[i] = (*p1)[i] ^ (*p2)[i];
	}
}
void bitsetXOR32(bitset<32> * dest, bitset<32> * p1, bitset<32> *p2)
{
	int i;

	for (i=0; i<32; i++)
	{
		(*dest)[i] = (*p1)[i] ^ (*p2)[i];
	}
}
void bitsetXOR48(bitset<48> * dest, bitset<48> * p1, bitset<48> *p2)
{
	int i;

	for (i=0; i<48; i++)
	{
		(*dest)[i] = (*p1)[i] ^ (*p2)[i];
	}
}
void bitsetXOR64(bitset<64> * dest, bitset<64> * p1, bitset<64> *p2)
{
	int i;

	for (i=0; i<64; i++)
	{
		(*dest)[i] = (*p1)[i] ^ (*p2)[i];
	}
}

void substitutie(bitset<32> * C, bitset<6> * B_u)
{
	bitset<2> linie;
	bitset<4> coloana, temp;
	int i;

	linie[0] = (*B_u)[0];
	linie[1] = (*B_u)[5];

	coloana[0] = (*B_u)[1];
	coloana[1] = (*B_u)[2];
	coloana[2] = (*B_u)[3];
	coloana[3] = (*B_u)[4];

	switch(whichB)
	{
	case 0: temp = bitset<4>(M1[bitsetToInt2(&linie)][bitsetToInt4(&coloana)]); break;
	case 1: temp = bitset<4>(M2[bitsetToInt2(&linie)][bitsetToInt4(&coloana)]); break;
	case 2: temp = bitset<4>(M3[bitsetToInt2(&linie)][bitsetToInt4(&coloana)]); break;
	case 3: temp = bitset<4>(M4[bitsetToInt2(&linie)][bitsetToInt4(&coloana)]); break;
	case 4: temp = bitset<4>(M5[bitsetToInt2(&linie)][bitsetToInt4(&coloana)]); break;
	case 5: temp = bitset<4>(M6[bitsetToInt2(&linie)][bitsetToInt4(&coloana)]); break;
	case 6: temp = bitset<4>(M7[bitsetToInt2(&linie)][bitsetToInt4(&coloana)]); break;
	case 7: temp = bitset<4>(M8[bitsetToInt2(&linie)][bitsetToInt4(&coloana)]); break;
	default: break;
	}

	for (i=0; i<4; i++)
		(*C)[whichB*4+i] = temp[i];


	whichB++;
}


void substitutie(bitset<6> * B_u, int whichB, bitset<4> * rez)
{
	bitset<2> linie;
	bitset<4> coloana, temp;
	int i;

	linie[0] = (*B_u)[0];
	linie[1] = (*B_u)[5];

	coloana[0] = (*B_u)[1];
	coloana[1] = (*B_u)[2];
	coloana[2] = (*B_u)[3];
	coloana[3] = (*B_u)[4];

	switch(whichB)
	{
	case 0: temp = bitset<4>(M1[bitsetToInt2(&linie)][bitsetToInt4(&coloana)]); break;
	case 1: temp = bitset<4>(M2[bitsetToInt2(&linie)][bitsetToInt4(&coloana)]); break;
	case 2: temp = bitset<4>(M3[bitsetToInt2(&linie)][bitsetToInt4(&coloana)]); break;
	case 3: temp = bitset<4>(M4[bitsetToInt2(&linie)][bitsetToInt4(&coloana)]); break;
	case 4: temp = bitset<4>(M5[bitsetToInt2(&linie)][bitsetToInt4(&coloana)]); break;
	case 5: temp = bitset<4>(M6[bitsetToInt2(&linie)][bitsetToInt4(&coloana)]); break;
	case 6: temp = bitset<4>(M7[bitsetToInt2(&linie)][bitsetToInt4(&coloana)]); break;
	case 7: temp = bitset<4>(M8[bitsetToInt2(&linie)][bitsetToInt4(&coloana)]); break;
	default: break;
	}

	for (i=0; i<4; i++)
		(*rez)[whichB*4+i] = temp[i];

}


void keyEncryption(bitset<32> * dest, bitset<32> * A, bitset<48> * crtKey)   // sau asa zisa functie F
{
	bitset<48> EA; // expanded A - aici fac marirea A-ului
	bitset<48> xoredEA;
	bitset<6> B1, B2, B3, B4, B5, B6, B7, B8, * crtB;
	bitset<32> temp;
	int i, j;

	// Expansiunea din A in E(A)  ...  sau EA cum am denumit bitset-ul
	for (i=0; i<48; i++)
	{
		EA[i] = (*A)[E[i]-1];
	}

	//xoram cu cheia
	bitsetXOR48(&xoredEA, &EA, crtKey);

	//spargem xoredEA in cele 8 bucati pentru substitutie
	for (i=0; i<8; i++)
	{
		switch (i)
		{
		case 0: crtB = &B1; break;
		case 1: crtB = &B2; break;
		case 2: crtB = &B3; break;
		case 3: crtB = &B4; break;
		case 4: crtB = &B5; break;
		case 5: crtB = &B6; break;
		case 6: crtB = &B7; break;
		case 7: crtB = &B8; break; 
		}

		for (j=0; j<6; j++)
		{
			(*crtB)[j] = xoredEA[48-((i+1)*6)+ j];
		}
	}


	//facem substitutiile
	whichB = 0;
	substitutie(dest, &B1);
	substitutie(dest, &B2);
	substitutie(dest, &B3);
	substitutie(dest, &B4);
	substitutie(dest, &B5);
	substitutie(dest, &B6);
	substitutie(dest, &B7);
	substitutie(dest, &B8);
	// in momentul de fata in "dest" se afla bitstring-ul C de 32 de biti


	// Permutarea finala pentru C
	for (i=0; i<32; i++)
	{
		temp[i] = (*dest)[P[i]-1];
	}

	for (i=0; i<32; i++)
		(*dest)[i] = temp[i];

}





bool DES_CD::CryptBitset(bitset<64> toCrypt, bitset<64> * crypted, bool toDecrypt)
{
	bitset<64> x0, temp;
	bitset<32> LCrt, RCrt, LNext, RNext, TheC;
	bitset<48> crtKey;
	int i;

	// permutarea initiala
	for (i=0; i<64; i++)
	{
		x0[i] = toCrypt[IP[i]-1];
	}
	
	// sectionare initiala
	splitBitsetInHalf (&x0, &LCrt, &RCrt);

	// pentru fiecare runda
	for (i=1; i<=NrRunde; i++)
	{
		if (!toDecrypt)
			getCrtKey (&crtKey, i);
		else
			getCrtKey (&crtKey, (NrRunde-i+1));

		LNext = RCrt;
		keyEncryption(&TheC, &RCrt, &crtKey);
		bitsetXOR32(&RNext, &LCrt, &TheC);

		RCrt = RNext;
		LCrt = LNext;
	}


	//aici construiesc inapoi din cele 2 bucati
	joinBitsetFromHalfs (crypted, &RNext, &LNext);

	//permutare inversa finala
	for (i=0; i<64; i++)
		temp[i] = (*crypted)[IPInv[i]-1];

	for (i=0; i<64; i++)
		(*crypted)[i] = temp[i];

	return true;
}


bool DES_CD::Crypt()
{
	int i, j, k;
	unsigned long int * converter;
	string temp;
	bitset<64> crypted, tocrypt;
	bitset<8> piece;

	this->cryptoText = "";

	for (i=0; i<(plainText.size()/8); i++)
	{
		temp = "";
		for (j=0; j<8; j++)
			temp += plainText[i*8+j];

		stringToBitset(temp, &tocrypt);

		CryptBitset(tocrypt, &crypted, false);

		temp = "";
		bitsetToString(&crypted, &temp);

		this->cryptoText += temp;
	}

	return true;
}

bool DES_CD::Decrypt()
{
	int i, j, k;
	unsigned long int * converter;
	string temp;
	bitset<64> crypted, tocrypt;
	bitset<8> piece;

	this->plainText = "";

	for (i=0; i<(cryptoText.size()/8); i++)
	{
		temp = "";
		for (j=0; j<8; j++)
			temp += cryptoText[i*8+j];


		stringToBitset(temp, &tocrypt);

		CryptBitset(tocrypt, &crypted, true);

		temp = "";
		bitsetToString(&crypted, &temp);

		this->plainText += temp;
	}

	return true;
}

bool DES_CD::Attack(const char * filepath)
{
	int n, i, j, max, indice;
	int *careJ;

	bitset<64> PlainCrt1, CryptCrt1, PlainCrt2, CryptCrt2, key;
	string plain1, plain2, crypt1, crypt2;
	bitset<48> prekey;
	bitset<56> prekey56;
	bitset<6> KP1, KP2, KP3, KP4, KP5, KP6, KP7, KP8;
	string buffer;
	string keyBuff;

	for (i=0; i<64; i++)
	{
		J1[i] = 0;
		J2[i] = 0;
		J3[i] = 0;
		J4[i] = 0;
		J5[i] = 0;
		J6[i] = 0;
		J7[i] = 0;
		J8[i] = 0;
	}

	ifstream fin(filepath);
	fin>>n;
	for (i=0; i<n; i++)
	{
		fin>>buffer;
		plain1 = buffer;
		stringToBitset(buffer, &PlainCrt1);
		buffer = "";

		fin>>buffer;
		crypt1 = buffer;
		stringToBitset(buffer, &CryptCrt1);
		buffer = "";

		fin>>buffer;
		plain2 = buffer;
		stringToBitset(buffer, &PlainCrt2);
		buffer = "";

		fin>>buffer;
		crypt2 = buffer;
		stringToBitset(buffer, &CryptCrt2);
		buffer = "";

		counterPossibleKeys(&PlainCrt1, &CryptCrt1, &PlainCrt2, &CryptCrt2);
	}

	for (i=0; i<n; i++)
	{
		switch(i)
		{
			case 0: careJ = J1;
			case 1: careJ = J2;
			case 2: careJ = J3;
			case 3: careJ = J4;
			case 4: careJ = J5;
			case 5: careJ = J6;
			case 6: careJ = J7;
			case 7: careJ = J8;
			default: break;
		}

		//selectam cea mai probabila
		max = 0;
		indice = -1;
		for (j=0; j<n; j++)
		{
			if (careJ[j] > max)
			{
				max = careJ[j];
				indice = j;
			}
		}

		// indice = valoarea lui J
		// de-aci incolo e vorba de inversarea procesului cu PC1 si PC2 si apoi o cautare exaustiva pentru restul.
	}


	for (i=0, max=0, indice=0; i<64; i++)
		if (J1[i] > max)
		{
			max = J1[i];
			indice = i;
		}
	KP1 = bitset<6>(indice);
	for (i=0, max=0, indice=0; i<64; i++)
		if (J2[i] > max)
		{
			max = J2[i];
			indice = i;
		}
	KP2 = bitset<6>(indice);
	for (i=0, max=0, indice=0; i<64; i++)
		if (J3[i] > max)
		{
			max = J3[i];
			indice = i;
		}
	KP3 = bitset<6>(indice);
	for (i=0, max=0, indice=0; i<64; i++)
		if (J4[i] > max)
		{
			max = J4[i];
			indice = i;
		}
	KP4 = bitset<6>(indice);
	for (i=0, max=0, indice=0; i<64; i++)
		if (J5[i] > max)
		{
			max = J5[i];
			indice = i;
		}
	KP5 = bitset<6>(indice);
	for (i=0, max=0, indice=0; i<64; i++)
		if (J6[i] > max)
		{
			max = J6[i];
			indice = i;
		}
	KP6 = bitset<6>(indice);
	for (i=0, max=0, indice=0; i<64; i++)
		if (J7[i] > max)
		{
			max = J7[i];
			indice = i;
		}
	KP7 = bitset<6>(indice);
	for (i=0, max=0, indice=0; i<64; i++)
		if (J8[i] > max)
		{
			max = J8[i];
			indice = i;
		}
	KP8 = bitset<6>(indice);

	// get prekey
	for (i=0; i<48; i++)
	{
		switch ((int)floor((double)(i/6)))
		{
		case 0:
			careJ = J1;
			break;
		case 1:
			careJ = J2;
			break;
		case 2:
			careJ = J3;
			break;
		case 3:
			careJ = J4;
			break;
		case 4:
			careJ = J5;
			break;
		case 5:
			careJ = J6;
			break;
		case 6:
			careJ = J7;
			break;
		case 7:
			careJ = J8;
			break;
		default: break;
		}
		prekey[i] = careJ[i%6];
	}

	// resize la 56
	for (i=0; i<48; i++)
	{
		prekey56[PC2[i]-1] = prekey[i];
	}


	//brutforce
		//shiftari


		//resize la 64
		for (i=0; i<56; i++)
		{
			key[PC1[i]-1] = prekey56[i];
		}
	

	bitsetToString(&key, &keyBuff);
	this->SetKey(keyBuff);
	this->SetPlaintext(plain1);

	return true;
}



void counterPossibleKeys(bitset<64> * PlainCrt1, bitset<64> * CryptCrt1, bitset<64> * PlainCrt2, bitset<64> * CryptCrt2)
{
	bitset<48> Ecrt, EcrtStea;
	bitset<32> C_out;
	bitset<32> R3Prim, L0Prim;
	bitset<32> LT1, RT1, LT2, RT2;
	int i;

	//SBOX inputs
	for (i=0; i<48; i++)
	{
		Ecrt[i] = (*CryptCrt1)[E[i]-1];
		EcrtStea[i] = (*CryptCrt2)[E[i]-1];
	}

	//SBOX outputs
	splitBitsetInHalf(CryptCrt1, &LT1, &RT1);
	splitBitsetInHalf(CryptCrt2, &LT2, &RT2);
	bitsetXOR32 (&R3Prim, &RT1, &RT2);

	splitBitsetInHalf(PlainCrt1, &LT1, &RT1);
	splitBitsetInHalf(PlainCrt2, &LT2, &RT2);
	bitsetXOR32 (&L0Prim, &LT1, &LT2);

	bitsetXOR32(&C_out, &L0Prim, &R3Prim);

	for (i=0; i<32; i++)
	{
		C_out[i] = C_out[invP[i]-1];
	}

	doTestFunction (&Ecrt, &EcrtStea, &C_out);
}

void doTestFunction (bitset<48> * Ecrt, bitset<48> * EcrtStea, bitset<32> * C_out)
{
	vector<bitset<6>> inList[8];
	bitset<6> Bj, Ej;
	bitset<6> BjprimBj, Bjprim, testJ;
	bitset<4> Cjprim, SjBj, SjBjprim, BjXor;
	bitset<48> Estea;
	int i, j, nr;
	int * careJ;

	bitsetXOR48(&Estea, Ecrt, EcrtStea);
	// generam tot inList-ul
	for (j=0; j<8; j++)
	{
		for (i=0; i<4; i++)
		{
			Cjprim[i] = (*C_out)[j*4+i];
		}

		for (i=0; i<6; i++)
		{
			Bjprim[i] = Estea[j*6+i];
		}


		for (i=0; i<64; i++)
		{
			Bj = bitset<6>(i);

			substitutie(&Bj, j, &SjBj);
			bitsetXOR6(&BjprimBj, &Bj, &Bjprim);
			substitutie(&BjprimBj, j, &SjBjprim);

			bitsetXOR4(&BjXor, &SjBj, &SjBjprim);

			if (BjXor == Cjprim)
			{
				inList[j].insert(inList[j].end(), Bj);
			}
		}
	}

	for (j=0; j<8; j++)
	{
		for (i=0; i<6; i++)
		{
				Ej[i] = (*Ecrt)[j*6+i];
		}

		switch(j)
		{
			case 0: careJ = J1;
			case 1: careJ = J2;
			case 2: careJ = J3;
			case 3: careJ = J4;
			case 4: careJ = J5;
			case 5: careJ = J6;
			case 6: careJ = J7;
			case 7: careJ = J8;
			default: break;
		}

		for (i=0; i<inList[j].size(); i++)
		{
			bitsetXOR6(&testJ, &Ej, &inList[j][i]);
			nr = bitsetToInt6(&testJ);

			careJ[nr] ++;
		}
	}

}




void stringToBitset(string str, bitset<64> * bit)
{
	int j, k;
	bool a;
	for (j=0; j<8; j++)
	{
		for (k=0; k<8; k++)
		{
			a = CHECK_BIT(str[j], k);
			if (a)
				(*bit).set(64-((j+1)*8)+k);
		}
	}
}

void bitsetToString(bitset<64> * bit, string * str)
{
	int j,k;
	bitset<8> piece;

	(*str) = "";

	for (j=0; j<8; j++)
	{
		for (k=0; k<8; k++)
		{
			piece[k] = (*bit)[64- ((j+1)*8)+ k];
		}

		(*str) += bitsetToChar(piece);
	}
}

long int bitsetToInt(bitset<64> * bit)
{
	long int c;
	int i;
	c = 0;

	for (i=0; i<64; i++)
	{
		if ((*bit)[i])
			SET_BIT(c, i);
	}

	return c;
}

long int bitsetToInt2(bitset<2> * bit)
{
	long int c;
	int i;
	c = 0;

	for (i=0; i<2; i++)
	{
		if ((*bit)[i])
			SET_BIT(c, i);
	}

	return c;
}
long int bitsetToInt4(bitset<4> * bit)
{
	long int c;
	int i;
	c = 0;

	for (i=0; i<4; i++)
	{
		if ((*bit)[i])
			SET_BIT(c, i);
	}

	return c;
}
long int bitsetToInt6(bitset<6> * bit)
{
	long int c;
	int i;
	c = 0;

	for (i=0; i<6; i++)
	{
		if ((*bit)[i])
			SET_BIT(c, i);
	}

	return c;
}


unsigned char bitsetToChar(bitset<8> & bit)
{
	unsigned char c;
	int i;
	c = 0;

	for (i=0; i<8; i++)
	{
		if ( bit.test(i) )
			if (bit.test(i))
				c = c | (1 << (i));
	}

	return c;
}




