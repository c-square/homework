#include <iostream>
#include <stdlib.h>
#include <vector>
#include <array>
#include "Punct.h"
using namespace std;

#ifndef POLIGON
#define POLIGON

class Poligon
{
	int nrPct;
	vector<Punct> P;
	
public:

	Poligon(int=0, vector<Punct>& = vector<Punct>());		// constructor
	~Poligon(){}			// destructor
	
	int getNrPct();
	vector<Punct>& getPcts();

	bool convex();
	double arie();
	
};

istream& operator>>(istream& i, Poligon& t);
ostream& operator<<(ostream& o, Poligon& t);

#endif
