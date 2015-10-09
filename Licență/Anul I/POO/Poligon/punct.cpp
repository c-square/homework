#include "punct.h"
#include <iostream>
#include <math.h>
using namespace std;


	/* ======== Din interiorul clasei ============= */
	//Constructor
	Punct::Punct(int X, int Y)
	{
		this->X = X;
		this->Y = Y;
	}
	
	int  Punct::GetX() const {     return this->X;    } 			// returneaza coordonata pe orizontala
	int  Punct::GetY() const {     return this->Y;    }				// returneaza coordonata pe verticala

	void Punct::MutaX (int x) {    this->X = x;       } 		// deplaseaza punctul pe orizontala
	void Punct::MutaY (int y) {    this->Y = y;       } 		// deplaseaza punctul pe verticala
	void Punct::MutaXY(int x, int y){    this->X = x;    this->Y = y;   }      	// deplaseaza punctul pe orizontala si verticala

	bool Punct::isInOrigine() { return (*this)==Origine; }
	bool Punct::isEgal(const Punct& a) { return (a.GetX() == this->X)&&(a.GetY() == this->Y);  }

	void Punct::interschimba(Punct& a) 
	{
		int X, Y;

		X = this->X;
		Y = this->Y;
		this->X = a.X;
		this->Y = a.Y;
		a.X = X;
		a.Y = Y;
	}

	double Punct::distanta(const Punct& a)
	{      return sqrt( (a.X-this->X)*(a.X-this->X) + (a.Y-this->Y)*(a.Y-this->Y)  );       }


	void Punct::operator=(int a)
	{
		this->X = a;
		this->Y = a;
	}

	bool Punct::isColiniar(const Punct& a, const Punct& b)
	{
		if ( this->GetX()*a.GetY() + a.GetX()*b.GetY() + b.GetX()*this->GetY() - a.GetY()*b.GetX() - this->GetX()*b.GetY() - a.GetX()*this->GetY() == 0)
			return 1;
		return 0;
	}


	// constante statice
	const Punct Punct::Origine;
	const Punct Punct::UnuZero(1);
	const Punct Punct::ZeroUnu(0,1);

	/* ============== === ================ */





	//globale
	bool puncteEgale (const Punct& a, const Punct& b) 
	{          return a==b;             }

	void detDreptunghi (const Punct& a, const Punct& b, Punct* v)
	{
		v[0].MutaX(a.GetX());
		v[0].MutaY(b.GetY());
		v[1].MutaX(b.GetX());
		v[1].MutaY(a.GetY());
	}



	// operatori
	istream& operator>>(istream& i, Punct& p)
	{
		int x,y;
		i>>x>>y;
		p = Punct(x,y);
		return i;
	}

	ostream& operator<<(ostream& o, Punct& p)
	{
		o<<'('<<p.GetX()<<", "<<p.GetY()<<")";
		return o;
	}

	bool operator==(const Punct& a, const Punct& b) { return (a.GetX()==b.GetX()&&a.GetY()==b.GetY()); }
	bool operator!=(const Punct& a, const Punct& b) { return !(a.GetX()==b.GetX()&&a.GetY()==b.GetY()); }
	
