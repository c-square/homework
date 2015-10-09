#include "Biblioteca.h"
#include <string>
#include <vector>
#include <stdlib.h>
using namespace std;


Publicatie::Publicatie() {}
Publicatie::Publicatie(std::string titlu, std::string editura, int an, int tiraj, int id)
{
	this->id = id;
	this->titlu = titlu;
	this->editura = editura;
	this->an = an;
	this->tiraj = tiraj;
}

std::string& Publicatie::getTitlu() {  return this->titlu;  }
std::string& Publicatie::getEditura() { return this->editura; }
int Publicatie::getAn() {   return this->an;    }
int Publicatie::getTiraj(){ return this->tiraj; }
int Publicatie::getCota(){  return this->cota;  }
int Publicatie::getId(){  return this->id;  }

void Publicatie::setTitlu(std::string& titlu) {  this->titlu = titlu;  } 
void Publicatie::setEditura(std::string& editura) {   this->editura = editura;   }
void Publicatie::setAn(int an) {  this->an = an;   }
void Publicatie::setTiraj(int tiraj) {  this->tiraj = tiraj;   }
void Publicatie::setCota(int cota){   this->cota = cota;    }
void Publicatie::setId(int id){ this->id = id; }






	
Carte::Carte() {}
Carte::Carte(vector<std::string>& autori) {  this->autori.swap(autori);  }

void Carte::adaugaAutor(std::string& autor)     {  this->autori.insert(this->autori.end(), autor);   }
vector<std::string>& Carte::getAutori()         {  return this->autori;    }
ostream& operator<<(ostream& o, Carte& t)
{
	int i;

	o<<"c{ "<<"An: "<<t.getAn()<<"\n";
	o<<"Editura: "<<t.getEditura()<<"\n";
	o<<"Titlu: "<<t.getTitlu()<<"\n";
	o<<"Tiraj: "<<t.getTiraj()<<"\n";
	o<<"Autori: "<<t.getAutori().size()<<"\n";
	for (i=0; i<t.getAutori().size(); i++)
	{
		o<<t.getAutori()[i]<<"\n";
	}
	o<<"ID: "<<t.getId()<<"\n";
	o<<"}\n";
	return o;
}
istream& operator>>(istream& i, Carte& t)
{
	std::string tempstr;
	int tempint, nr, j;
	char umplutura[50];
	

	i.seekg (7, std::ios::cur);
	i>>tempint;
	i.seekg (1, std::ios::cur);
	t.setAn(tempint);

	i.seekg (9, std::ios::cur);
	i>>tempstr;
	i.seekg (1, std::ios::cur);
	t.setEditura(tempstr);

	i.seekg (7, std::ios::cur);
	i>>tempstr;
	i.seekg (1, std::ios::cur);
	t.setTitlu(tempstr);

	i.seekg (7, std::ios::cur);
	i>>tempint;
	i.seekg (1, std::ios::cur);
	t.setTiraj(tempint);

	i.seekg (8, std::ios::cur);
	i>>nr;
	i.seekg (1, std::ios::cur);
	for (j=0; j<nr; j++)
	{
		i>>tempstr;
		i.seekg (1, std::ios::cur);
		t.getAutori().insert(t.getAutori().end(), tempstr);
	}
	i.seekg (4, std::ios::cur);
	i>>tempint;
	i.seekg (4, std::ios::cur);

	return i;
}



	
Revista::Revista() {}
Revista::Revista(int nr, int frecv) {  this->nr = nr;   this->frecvAparitie = frecv;  }

void Revista::setNr(int nr) {  this->nr = nr;   }
void Revista::setFrecv(int frecv) {   this->frecvAparitie = frecv;  }

int Revista::getNr() {   return this->nr;   }
int Revista::getFrecv() {   return this->frecvAparitie;    }
ostream& operator<<(ostream& o, Revista& t)
{
	int i;

	o<<"r{ "<<"An: "<<t.getAn()<<"\n";
	o<<"Editura: "<<t.getEditura()<<"\n";
	o<<"Titlu: "<<t.getTitlu()<<"\n";
	o<<"Tiraj: "<<t.getTiraj()<<"\n";
	o<<"NR.: "<<t.getNr()<<"\n";
	o<<"Frecventa: "<<t.getFrecv()<<"\n";
	o<<"ID: "<<t.getId()<<"\n";
	o<<"}\n";
	return o;
}
istream& operator>>(istream& i, Revista& t)
{
	std::string tempstr;
	int tempint;
	char umplutura[50];

	i.seekg (7, std::ios::cur);
	i>>tempint;
	i.seekg (1, std::ios::cur);
	t.setAn(tempint);

	i.seekg (9, std::ios::cur);
	i>>tempstr;
	i.seekg (1, std::ios::cur);
	t.setEditura(tempstr);

	i.seekg (7, std::ios::cur);
	i>>tempstr;
	i.seekg (1, std::ios::cur);
	t.setTitlu(tempstr);

	i.seekg (7, std::ios::cur);
	i>>tempint;
	i.seekg (1, std::ios::cur);
	t.setTiraj(tempint);

	i.seekg (5, std::ios::cur);
	i>>tempint;
	i.seekg (1, std::ios::cur);
	t.setNr(tempint);

	i.seekg (11, std::ios::cur);
	i>>tempint;
	i.seekg (1, std::ios::cur);
	t.setFrecv(tempint);

	i.seekg (4, std::ios::cur);
	i>>tempint;
	i.seekg (4, std::ios::cur);

	return i;
}




Biblioteca::Biblioteca() {  this->nrPublicatii = 0;  this->crtID = 1; this->cartiDistincte = 0; this->revDistincte = 0; }
	
void Biblioteca::adaugaRevista (Revista& rev) 
{  
	int i;
	bool ok = true;
	rev.setId(this->crtID);
	this->crtID++;

	for (i=0; i<this->reviste.size(); i++)
	{
		if (rev.getAn() == reviste[i].getAn() && rev.getEditura() == reviste[i].getEditura() && rev.getFrecv() == reviste[i].getFrecv() && rev.getNr() == reviste[i].getNr() && rev.getTiraj() == reviste[i].getTiraj() && rev.getTitlu() == reviste[i].getTitlu() )
		{
			ok = false;
			break;
		}
	}
	if (ok) this->revDistincte++;

	this->reviste.insert(this->reviste.end(), rev);  

	this->nrPublicatii++; 
}
void Biblioteca::adaugaCarte (Carte& carte) 
{  
	int i, j;
	bool ok = true, ok2 = true;
	carte.setId(this->crtID);
	this->crtID++;

	for (i=0; i<this->carti.size(); i++)
	{
		if (carte.getAn() == carti[i].getAn() && carte.getEditura() == carti[i].getEditura() && carte.getTiraj() == carti[i].getTiraj() && carte.getTitlu() == carti[i].getTitlu() )
		{
			ok2 = true;
			if (carte.getAutori().size() == carti[i].getAutori().size())
				for (j=0; j<carte.getAutori().size(); j++)
					if (carte.getAutori()[j] != carti[i].getAutori()[j])
					{
						ok2 = false;
						break;
					}
			if (ok2)
			{
				ok = false;
				break;
			}
		}
	}
	if (ok) this->cartiDistincte++;

	this->carti.insert(this->carti.end(), carte);  
	
	this->nrPublicatii++;  
}

vector<Revista>& Biblioteca::getReviste()  {  return this->reviste; }
vector<Carte>& Biblioteca::getCarti()  { return this->carti;  }
int Biblioteca::getNrCarti()           { return (int)this->carti.size(); }
int Biblioteca::getNrReviste()         { return (int)this->reviste.size(); }
int Biblioteca::getNrPublicatii()      { return this->nrPublicatii; }
int Biblioteca::getNrCartiDistincte()  { return this->cartiDistincte; }
int Biblioteca::getNrRevDistincte()    { return this->revDistincte;   }

ostream& operator<<(ostream& o, Biblioteca& b)
{
	int i;

	o<<"c: "<<b.getNrCarti()<<"\n";
	for (i=0; i<b.getNrCarti(); i++)
		o<<b.getCarti()[i];

	o<<"r: "<<b.getNrReviste()<<"\n";
	for (i=0; i<b.getNrReviste(); i++)
		o<<b.getReviste()[i];

	return o;
}

istream& operator>>(istream& i, Biblioteca& b)
{
	int j, nr;
	Revista * rev;
	Carte * carte;
	//char umplutura[25];

	i.seekg (3, std::ios::cur);
	i>>nr;
	i.seekg (1, std::ios::cur);

	for (j=0; j<nr; j++)
	{
		carte = new Carte();
		i>>(*carte);
		b.adaugaCarte(*carte);
	}

	i.seekg (3, std::ios::cur);
	i>>nr;
	i.seekg (1, std::ios::cur);

	for (j=0; j<nr; j++)
	{
		rev = new Revista();
		i>>(*rev);
		b.adaugaRevista(*rev);
	}
	
	return i;
}



