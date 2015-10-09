#include <iostream>
#include "Punct.h"
using namespace std;

#ifndef TRIUNGHI
#define TRIUNGHI

class Triunghi
{
	Punct a, b, c;				// coordonatele triunghiului
	
public:

	Triunghi(Punct=0, Punct=0, Punct=0);		// constructor
	~Triunghi(){}			// destructor
	
	Punct& getPunct(char);
	double lgLatura(char);
	double perimetru();
	double arie ();
	
	bool dreptunghic();
	bool asemenea(Triunghi& t);
};

istream& operator>>(istream& i, Triunghi& t);
ostream& operator<<(ostream& o, Triunghi& t);

#endif
