#include <stdlib.h>
using namespace std;

// file: ArboreBinar.h
#ifndef ARBOREBINAR
#define ARBOREBINAR


#include <vector>
#include <string>
class ArboreBinar
{
	char cOperator;
	double dNumber;
	std::vector<ArboreBinar*> fii;
	bool valid;

	ArboreBinar(double); // constructor pentru initializarea unui nod frontiera
	ArboreBinar(char); // constructor pentru initializarea unui nod intern

	// metoda pentru transformarea in forma postfixata a unei expresii
	static std::string InfixToPostfix( const std::string& expresie );

	void adaugaFiu(ArboreBinar*);

public:
	// constructor pentru initializarea nodului radacina
	ArboreBinar( const std::string& expresie );

	// returneaza true daca arborele a fost construit (expresia era corecta)
	bool EsteValid();

	// returneaza expresia rezultata in urma parcurgerii arborelui
	// (inordine, preordine, postordine)
	std::string Parcurgere( int nCodParcurgere );

	// returneaza rezultatul evaluarii arborelui
	double Evalueaza();
};
#endif