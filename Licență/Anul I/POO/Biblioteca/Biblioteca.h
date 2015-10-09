#include <string>
#include <vector>
#include <stdlib.h>
using namespace std;

class Publicatie
{
	int cota;
	int id;

	std::string titlu;
	std::string editura;
	int an;
	int tiraj;

public:
	Publicatie();
	Publicatie(std::string, std::string, int, int, int);

	std::string& getTitlu();
	std::string& getEditura();
	int getAn();
	int getTiraj();
	int getCota();
	int getId();

	void setTitlu(std::string&);
	void setEditura(std::string&);
	void setAn(int);
	void setTiraj(int);
	void setCota(int);
	void setId(int);
};




class Carte: public Publicatie
{
	vector<std::string> autori;
public:
	Carte();
	Carte(vector<std::string>&);

	void adaugaAutor(std::string&);
	vector<std::string>& getAutori();
};
ostream& operator<<(ostream& o, Carte& t);
istream& operator>>(istream& i, Carte& t);


class Revista: public Publicatie
{
	int nr;
	int frecvAparitie;
public:
	Revista();
	Revista(int, int);

	void setNr(int);
	void setFrecv(int);

	int getNr();
	int getFrecv();
};
ostream& operator<<(ostream& o, Revista& t);
istream& operator>>(istream& i, Revista& t);

class Biblioteca
{
	int nrPublicatii;
	int crtID;
	int revDistincte;
	int cartiDistincte;
	vector<Revista> reviste;
	vector<Carte> carti;
public:
	Biblioteca();
	
	void adaugaRevista (Revista&);
	void adaugaCarte (Carte&);

	vector<Revista>& getReviste();
	vector<Carte>& getCarti();
	int getNrCarti();
	int getNrReviste();
	int getNrPublicatii();
	int getNrCartiDistincte();
	int getNrRevDistincte();
};
ostream& operator<<(ostream& o, Biblioteca& t);
istream& operator>>(istream& i, Biblioteca& b);



