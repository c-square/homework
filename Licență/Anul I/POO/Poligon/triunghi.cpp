#include "triunghi.h"
#include "Punct.h"
#include <iostream>
#include <math.h>
using namespace std;

	//constructor
	Triunghi::Triunghi(Punct a, Punct b, Punct c)
	{
		this->a = a;
		this->b = b;
		this->c = c;
	}



	Punct& Triunghi::getPunct(char pct)
	{
		if (pct == 'a') return a;
		else if (pct == 'b') return b;
		else if (pct == 'c') return c;
	}







	double Triunghi::lgLatura(char latura)
	{
		if (latura == 'a') return a.distanta(b);
		else if (latura == 'b') return b.distanta(c);
		else if (latura == 'c') return c.distanta(a); 
	}

	double Triunghi::perimetru()
	{
		return a.distanta(b) + b.distanta(c) + c.distanta(a);
	}

	double Triunghi::arie ()
	{
		double sp = this->perimetru() /2;
		return sqrt (sp*(sp-a.distanta(b))*(sp-b.distanta(c))*(sp-c.distanta(a)));
	}
	








	bool Triunghi::dreptunghic()
	{
		double marja = 0.01;
		double la, lb, lc;
		la = a.distanta(b);
		lb = b.distanta(c);
		lc = c.distanta(a);

		if (la*la - (lb*lb + lc*lc) < marja) return 1;
		if (lb*lb - (la*la + lc*lc) < marja) return 1;
		if (lc*lc - (lb*lb + la*la) < marja) return 1;

		return 0;
	}

	bool Triunghi::asemenea(Triunghi& t)
	{
		double marja = 0.01;
		double raport;
		double la, lb, lc;
		la = a.distanta(b);
		lb = b.distanta(c);
		lc = c.distanta(a);

		raport = la/t.lgLatura('a');
		if ( (lb/t.lgLatura('b') - raport < marja) && (lc/t.lgLatura('c') - raport < marja) )
			return 1;
		return 0;
	}












	istream& operator>>(istream& i, Triunghi& t)
	{
		Punct x,y,z;
		i>>x>>y>>z;
		t = Triunghi(x,y,z);
		return i;
	}


	ostream& operator<<(ostream& o, Triunghi& t)
	{
		o<<"<"<<t.getPunct('a')<<t.getPunct('b')<<t.getPunct('c')<<">";
		return o;
	}