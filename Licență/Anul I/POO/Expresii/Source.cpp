#include "ArboreBinar.h"
#include <iostream>
#include <string>
using namespace std;


int main ()
{
	int i;
	double rez;
	char tempstr[200];
	ArboreBinar* x;
	string a;

	cout<<"Give me an expression:\n";
	cin.getline(tempstr, 199);
	a += tempstr;
	a += "\0";

	x = new ArboreBinar (a);
	if (x->EsteValid())
	{
		cout<<"\nExpression valid\n";

		rez = x->Evalueaza();
		cout<<"\nRezultat: "<<rez<<"\n";
		cout<<"Preordine: "<<x->Parcurgere(1)<<"\n";
		cout<<"Inordine: "<<x->Parcurgere(2)<<"\n";
		cout<<"Postordine: "<<x->Parcurgere(3)<<"\n";
	}
	else
		cout<<"\nExpression invalid\n";

	cin>>i;
	return 0;
}