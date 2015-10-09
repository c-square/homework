#include "Biblioteca.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string.h>

using namespace std;


void head()
{
	cout<<"================================\n";
	cout<<"========Biblioteca v1.0=========\n";
	cout<<"================================\n\n";
}

void meniu()
{
	cout<<"\nComenzi:\n";
	cout<<"1. Adauga publicatie\n";
	cout<<"2. Eliminare publicatie\n";
	cout<<"3. Afisare publicatie\n";
	cout<<"4. Cauta publicatie\n";
	cout<<"5. Raport publicatii distincte\n";
	cout<<"6. Save in file\n";
	cout<<"7. Read from file\n";
}

void citesteRevista(Revista * rev)
{
	int tempint;
	std::string tempstring;

	cin>>tempint;
	(*rev).setNr(tempint);
	cin>>tempint;
	(*rev).setFrecv(tempint);
	cin>>tempstring;
	(*rev).setTitlu(tempstring);
	cin>>tempstring;
	(*rev).setEditura(tempstring);
	cin>>tempint;
	(*rev).setAn(tempint);
	cin>>tempint;
	(*rev).setTiraj(tempint);
}
void citesteCarte (Carte * carte)
{
	int tempint, i;
	std::string tempstring;

	cin>>tempint;
	for (i=0; i<tempint; i++)
	{
		cin>>tempstring;
		(*carte).adaugaAutor(tempstring);
	}
	cin>>tempstring;
	(*carte).setTitlu(tempstring);
	cin>>tempstring;
	(*carte).setEditura(tempstring);
	cin>>tempint;
	(*carte).setAn(tempint);
	cin>>tempint;
	(*carte).setTiraj(tempint);
}

int findRevista(int id, Biblioteca& b)
{
	int i;

	for (i=0; i<b.getReviste().size(); i++)
		if (b.getReviste()[i].getId() == id)
			return i;
	return -1;
}

int findCarte(int id, Biblioteca& b)
{
	int i;

	for (i=0; i<b.getCarti().size(); i++)
		if (b.getCarti()[i].getId() == id)
			return i;
	return -1;
}

vector<int> findByTitle(std::string title, Biblioteca& b)
{
	int i;
	vector<int> v;

	for (i=0; i<b.getNrCarti(); i++)
		if (b.getCarti()[i].getTitlu() == title)
			v.insert(v.end(), b.getCarti()[i].getId());

	for (i=0; i<b.getNrReviste(); i++)
		if (b.getReviste()[i].getTitlu() == title)
			v.insert(v.end(), b.getReviste()[i].getId());

	return v;
}
vector<int> findByTiraj(int tiraj, Biblioteca& b)
{
	int i;
	vector<int> v;

	for (i=0; i<b.getNrCarti(); i++)
		if (b.getCarti()[i].getTiraj() == tiraj)
			v.insert(v.end(), b.getCarti()[i].getId());

	for (i=0; i<b.getNrReviste(); i++)
		if (b.getReviste()[i].getTiraj() == tiraj)
			v.insert(v.end(), b.getReviste()[i].getId());

	return v;
}
vector<int> findByEditura(std::string editura, Biblioteca& b)
{
	int i;
	vector<int> v;

	for (i=0; i<b.getNrCarti(); i++)
		if (b.getCarti()[i].getEditura() == editura)
			v.insert(v.end(), b.getCarti()[i].getId());

	for (i=0; i<b.getNrReviste(); i++)
		if (b.getReviste()[i].getEditura() == editura)
			v.insert(v.end(), b.getReviste()[i].getId());

	return v;
}
vector<int> findByAn(int an, Biblioteca& b)
{
	int i;
	vector<int> v;

	for (i=0; i<b.getNrCarti(); i++)
		if (b.getCarti()[i].getAn() == an)
			v.insert(v.end(), b.getCarti()[i].getId());

	for (i=0; i<b.getNrReviste(); i++)
		if (b.getReviste()[i].getAn() == an)
			v.insert(v.end(), b.getReviste()[i].getId());

	return v;
}


void scrieInFisier(Biblioteca& b)
{
	ofstream fout("biblioteca.out");
	fout<<b;
	fout.close();
}

void citesteDinFisier(Biblioteca& b)
{
	ifstream fin("biblioteca.out");
	fin>>b;
	fin.close();
}


int main ()
{
	Biblioteca b;
	int comanda = 0;
	int tip;
	int i;
	Revista * rev;
	Carte * carte;
	

	int tempint;
	std::string tempstring;
	vector<int> tempv;
	

	head();
	
	while (comanda!=-1)
	{
		meniu();
		cin>>comanda;
		if (comanda==1) //adauga publicatie
		{
			while(1)
			{
				cout<<"\n->Alege tip: 1. Revista; 2. Carte;\n->";
				cin>>tip;
				if (tip==1)
				{
					rev = new Revista();

					cout<<"\n->Informatii fiecare pe o linie: nr, frecventa, titlu, editura, an, tiraj";
					citesteRevista(rev);

					b.adaugaRevista(*rev);
					break;
				}
				else
				if (tip==2)
				{
					carte = new Carte();
					
					cout<<"\n->Informatii fiecare pe o linie: nrAutori, Autori, titlu, editura, an, tiraj";
					citesteCarte(carte);

					b.adaugaCarte(*carte);
					break;
				}
				else
					cout<<"   #Tip inexistent";
			}
		}
		else
		if (comanda==2)
		{
			cout<<"ID va rog:";
			cin>>i;//getid

			//verificare reviste
			tempint = findRevista(i, b);
			if (tempint!=-1) 
			{
				cout<<"Publicatie Revista stearsa";
				b.getReviste().erase(b.getReviste().begin()+tempint);
			}
			else
			{
				tempint = findCarte(i, b);
				if (tempint!=-1)
				{
					cout<<"Publicatie Carte stearsa";
					b.getCarti().erase(b.getCarti().begin()+tempint);
				}
				else
					cout<<"Publicatia nu a fost gasita";
			}			
		}
		else
		if (comanda==3)
		{
			cout<<"ID va rog:";
			cin>>i;//getid

			//verificare reviste
			tempint = findRevista(i, b);
			if (tempint!=-1) 
				cout<<b.getReviste()[tempint];
			else
			{
				tempint = findCarte(i, b);
				if (tempint!=-1)
					cout<<b.getCarti()[tempint];
				else
					cout<<"Publicatia nu a fost gasita";
			}
		}
		else
		if (comanda==4)
		{
			cout<<"\t 1. Titlu;  2. Tiraj;  3. Editura;  4. An";
			cin>>comanda;
			if (comanda==1)
			{
				cout<<"GiveMeTitle:";
				cin>>tempstring;
				tempv = findByTitle(tempstring, b);
			}
			else
			if (comanda==2)
			{
				cout<<"GiveMeTiraj:";
				cin>>tempint;
				tempv = findByTiraj(tempint, b);
			}
			else
			if (comanda==3)
			{
				cout<<"GiveMeEditura:";
				cin>>tempstring;
				tempv = findByEditura(tempstring, b);
			}
			else
			if (comanda==4)
			{
				cout<<"GiveMeAn:";
				cin>>tempint;
				tempv = findByAn(tempint, b);
			}

			if (tempv.size() == 0) cout<<"\n Nu a fost gasit nimic;\n";

			for (i=0; i<tempv.size(); i++)
			{
				tempint = findRevista(tempv[i], b);
				if (tempint!=-1) 
					cout<<b.getReviste()[tempint];
				else
				{
					tempint = findCarte(tempv[i], b);
					if (tempint!=-1)
						cout<<b.getCarti()[tempint];
				}
			}
		}
		else
		if (comanda==5)
		{
			cout<<"Raport:\n";
			cout<<"Publicatii: "<<b.getNrPublicatii()<<"\n";
			cout<<"Reviste: "<<b.getNrRevDistincte()<<"\n";
			cout<<"Carti: "<<b.getNrCartiDistincte()<<"\n";
		}
		else
		if (comanda==6)
		{
			scrieInFisier(b);
		}
		else
		if (comanda==7)
		{
			citesteDinFisier(b);
		}
	}

	return 0;
}
