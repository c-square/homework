#include <iostream>
#include <stdlib.h>
#include <stdarg.h>
#include <vector>
#include <math.h>
#include "Punct.h"
#include "poligon.h"
#include "triunghi.h"
using namespace std;





	/*Poligon::Poligon(int nrPct, ...)
	{
		va_list vl;
		int i;

		va_start(vl, nrPct);
		for (i=0; i<nrPct; i++)
		{
			P.insert(P.end(), va_arg( vl, Punct));
		}
		va_end (vl);

	}*/

	Poligon::Poligon(int nrPct, vector<Punct>& v)
	{
		this->nrPct = nrPct;
		this->P.swap (v);
	}
	
	int Poligon::getNrPct()			  {   return this->nrPct;	 }
	vector<Punct>& Poligon::getPcts() {   return this->P;        }



	bool Poligon::convex()
	{
		int i;
		int semn;
		int val;
		int dx1, dy1, dx2, dy2;
		
		i=0;
		dx1 = P[i+1].GetX()-P[i].GetX();
		dy1 = P[i+1].GetY()-P[i].GetY();
		dx2 = P[i+2].GetX()-P[i+1].GetX();
		dy2 = P[i+2].GetY()-P[i+1].GetY();
			
		val = dx1*dy2-dy1*dx2;
		if (val >= 0) semn = 1;
			else semn = 0;


		for (i=1; i<nrPct-2; i++)
		{
			dx1 = P[i+1].GetX()-P[i].GetX();
			dy1 = P[i+1].GetY()-P[i].GetY();
			dx2 = P[i+2].GetX()-P[i+1].GetX();
			dy2 = P[i+2].GetY()-P[i+1].GetY();
			
			val = dx1*dy2-dy1*dx2;

			if (val>0 && semn<0) return 0;
				else if (val<0 && semn>0) return 0;
		}


		dx1 = P[i+1].GetX()-P[i].GetX();
		dy1 = P[i+1].GetY()-P[i].GetY();
		dx2 = P[0].GetX()-P[i+1].GetX();
		dy2 = P[0].GetY()-P[i+1].GetY();
			
		val = dx1*dy2-dy1*dx2;

		if (val>0 && semn<0) return 0;
			else if (val<0 && semn>0) return 0;

		dx1 = P[0].GetX()-P[i].GetX();
		dy1 = P[0].GetY()-P[i].GetY();
		dx2 = P[1].GetX()-P[0].GetX();
		dy2 = P[1].GetY()-P[0].GetY();
			
		val = dx1*dy2-dy1*dx2;

		if (val>0 && semn<0) return 0;
			else if (val<0 && semn>0) return 0;


		return 0;
	}


	double Poligon::arie()
	{
		int i;
		double unX;
		double Arie = 0;
		Punct p = Punct();
		Triunghi t;

		for (i=0; i<nrPct-1; i++)
			if ( !p.isColiniar(P[i], P[i+1]))
			{
				unX = (double)(P[i+1].GetY() - p.GetY())*(P[i].GetX() - p.GetX())/(double)(P[i].GetY() - p.GetY()) + p.GetX(); // vad care ar fi trebuit sa fie X-ul celui de-al doilea punct a.i. el sa fi fost pe dreapta;
				t = Triunghi (p, P[i], P[i+1]);

				if (P[i+1].GetX() > unX)
					Arie += t.arie();
				else
					Arie -= t.arie();
			}


		if ( !p.isColiniar(P[i], P[0]))
		{
			unX = (double)(P[0].GetY() - p.GetY())*(P[i].GetX() - p.GetX())/(double)(P[i].GetY() - p.GetY()) + p.GetX(); // vad care ar fi trebuit sa fie X-ul celui de-al doilea punct a.i. el sa fi fost pe dreapta;
			t = Triunghi (p, P[i], P[0]);

			if (P[0].GetX() > unX)
				Arie += t.arie();
			else
				Arie -= t.arie();
		}
		
		return abs(Arie);
	}
	



	istream& operator>>(istream& i, Poligon& t)
	{
		vector<Punct> a;
		int nr, j;
		Punct p;

		i>>nr;
		for (j=0; j<nr; j++)
		{
			i>>p;
			a.insert (a.end(), p);
		}

		t = Poligon(nr, a);
		return i;
	}

	ostream& operator<<(ostream& o, Poligon& t)
	{
		int i;
		vector<Punct> p;

		p = t.getPcts();

		o<<"<";
		for (i=0; i<t.getNrPct(); i++)
		{
			o<<p.back();
			p.pop_back();
		}
		o<<">";

		return o;
	}