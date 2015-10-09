#include <iostream>
using namespace std;

#ifndef PUNCT
#define PUNCT

class Punct
{
	int X, Y;				// coordonatele punctului
	
public:

	static const Punct Origine;
	static const Punct UnuZero;
	static const Punct ZeroUnu;

	Punct(int X = 0, int Y = 0);		// constructor
	~Punct(){}			// destructor
	
	int  GetX() const;			// returneaza coordonata pe orizontala
	int  GetY() const;			// returneaza coordonata pe verticala

	void MutaX (int x = 1); 		// deplaseaza punctul pe orizontala
	void MutaY (int y = 1); 		// deplaseaza punctul pe verticala
	void MutaXY(int x = 1, int y = 1);	// deplaseaza punctul pe orizontala si verticala

	bool isInOrigine();
	bool isEgal(const Punct&);
	bool isColiniar(const Punct&, const Punct&);
	void interschimba(Punct&);
	double distanta(const Punct&);

	void operator=(int);
};

istream& operator>>(istream& i, Punct& p);
ostream& operator<<(ostream& o, Punct& p);

bool puncteEgale (const Punct&, const Punct&);
void detDreptunghi (const Punct&, const Punct&, Punct*);

bool operator==(const Punct&, const Punct&);
bool operator!=(const Punct&, const Punct&);

#endif
