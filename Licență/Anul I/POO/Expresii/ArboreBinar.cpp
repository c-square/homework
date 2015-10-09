#include <stdlib.h>
#include "ArboreBinar.h"
using namespace std;

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <stack>


string ArboreBinar::InfixToPostfix(const string& expresie)
{
	// implementarea unui algoritm de tip shunting-yard
	stringstream stream; // flux deschis simultan pentru scriere si citire
	stream << expresie; // scriem expresia in flux

	stack<string> aStack;
	string postExpresie;
	char chSpace = ' ';

	while ( stream )
	{
		string strNumar;
		string strOperator;
		char aChar = stream.peek(); // ce caracter urmeaza in flux?

		switch ( aChar )
		{
			case ' ' : stream.ignore(); // "sarim" peste spatiu
			continue;
			case '.' :
			case '0' :
			case '1' :
			case '2' :
			case '3' :
			case '4' :
			case '5' :
			case '6' :
			case '7' :
			case '8' :
			case '9' : 
				stream >> strNumar; // citim numarul (ca sir)
				postExpresie += strNumar; // scriem numarul
				postExpresie += chSpace;
				break;

			case '+' :
			case '-' :
				stream >> strOperator; // citim operatorul
				while ( (! aStack.empty()) && (( aStack.top() == "*") || (aStack.top() == "/") || (aStack.top() == "+") || (aStack.top() == "-")))
				{
					postExpresie += aStack.top(); // scriem operatorul
					postExpresie += chSpace;
					aStack.pop(); // eliminam din stiva
				}
				aStack.push( strOperator ); // introducem operatorul in stiva
				break;

			case '*' :
			case '/' :
				stream >> strOperator; // citim operatorul
				while ( (! aStack.empty()) && (( aStack.top() == "*") || (aStack.top() == "/")))
				{
					postExpresie += aStack.top(); // scriem operatorul
					postExpresie += chSpace;
					aStack.pop(); // eliminam din stiva
				} 
				aStack.push( strOperator ); // introducem operatorul in stiva
				break;

			case '(' : 
				stream.ignore();
				aStack.push( "(" ); // introducem '(' in stiva
				break;

			case ')' :
				stream.ignore();
				while ( (! aStack.empty()) && (aStack.top() != "(") )
				{
					postExpresie += aStack.top(); // scriem in post-expresie
					postExpresie += chSpace;
					aStack.pop(); // eliminam din stiva
				} 
				aStack.pop(); // eliminam '(' din stiva
				break;

			case EOF : continue;
			default : return "Eroare la parsarea expresiei!";
			}
		}
		// golim stiva
		while (! aStack.empty())
		{
			postExpresie += aStack.top();
			postExpresie += chSpace;
			aStack.pop();
		}
	return postExpresie;
}

ArboreBinar::ArboreBinar(double nr)
{
	this->dNumber = nr;
}

ArboreBinar::ArboreBinar(char op)
{
	this->cOperator = op;
}


ArboreBinar::ArboreBinar( const std::string& expresie )
{
	string postfix = ArboreBinar::InfixToPostfix(expresie); // flux deschis simultan pentru scriere si citire
	stack<ArboreBinar*> arbs;
	stringstream stream(postfix);

	while (stream)
	{
		string str;
		double a;
		ArboreBinar * a1, * a2;
		char aChar = stream.peek();		

		if (aChar == '\0') break;

		switch (aChar)
		{
			case ' ' : 
				stream.ignore(); // "sarim" peste spatiu
				continue;
			case '.' :
			case '0' :
			case '1' :
			case '2' :
			case '3' :
			case '4' :
			case '5' :
			case '6' :
			case '7' :
			case '8' :
			case '9' : 
				stream >> a; // citim numarul (ca sir)
				arbs.push (new ArboreBinar ((double)a));
				break;

			case '+' :
			case '*' :
			case '/' :
			case '-' :
				stream.ignore();

				a1 = arbs.top();
				arbs.pop();
				a2 = arbs.top();
				arbs.pop();

				arbs.push (new ArboreBinar (aChar));
				arbs.top()->adaugaFiu(a1);
				arbs.top()->adaugaFiu(a2);
				break;
			default:
				break;
		}
	}

	

	this->fii.swap(arbs.top()->fii);
	this->cOperator = arbs.top()->cOperator;
	this->dNumber = arbs.top()->dNumber;

	arbs.pop();

	if (arbs.empty())
		this->valid = true;
	else
		this->valid = false;

	//this->adaugaFiu (arbs.top());
}

	
bool ArboreBinar::EsteValid()  {     return this->valid;     }
void ArboreBinar::adaugaFiu(ArboreBinar* a) {     this->fii.insert(this->fii.end(), a);   }
	
std::string ArboreBinar::Parcurgere( int nCodParcurgere )
{
	
	if (this->fii.empty())
	{
		std::ostringstream strs;
		strs << this->dNumber;
		return strs.str();
	}
	else
	{
		string str;

		if (nCodParcurgere == 1)
		{
			str+=this->cOperator;
			str+=this->fii[0]->Parcurgere(1);
			str+=this->fii[1]->Parcurgere(1);
		}
		else
		if (nCodParcurgere == 2)
		{
			str+=this->fii[0]->Parcurgere(2);
			str+=this->cOperator;
			str+=this->fii[1]->Parcurgere(2);
		}
		else
		if (nCodParcurgere == 3)
		{
			str+=this->fii[0]->Parcurgere(3);
			str+=this->fii[1]->Parcurgere(3);
			str+=this->cOperator;
		}

		return str;
	}
}

double ArboreBinar::Evalueaza()
{
	if (this->fii.empty())
		return this->dNumber;
	else
		switch (this->cOperator)
		{

		case '+':
			return this->fii[0]->Evalueaza() + this->fii[1]->Evalueaza();
			break;
		case '*':
			return this->fii[0]->Evalueaza() * this->fii[1]->Evalueaza();
			break;
		case '-':
			return this->fii[0]->Evalueaza() - this->fii[1]->Evalueaza();
			break;
		case '/':
			return this->fii[0]->Evalueaza() / this->fii[1]->Evalueaza();
			break;

		default:
			return 0;
			break;
		}
}




