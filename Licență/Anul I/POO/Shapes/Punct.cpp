#include <iostream>
#include <ostream>
#include "Punct.h"

using namespace std;

	/*
	template <class TIP> Punct<TIP>::Punct(TIP a, TIP b)
	{
		this->x = a;
		this->y = b;
	}*/

	/*
	template <class TIP> TIP Punct<TIP>::getX() const {return this->x;}
	template <class TIP> TIP Punct<TIP>::getY() const {return this->y;}
	template <class TIP> void Punct<TIP>::setX(TIP a) {this->x = a;}
	template <class TIP> void Punct<TIP>::setY(TIP b) {this->y = b;}
	template <class TIP> void Punct<TIP>::moveBy(TIP a, TIP b) { this->x += a; this->y += b; }
	*/

	/*
	istream &operator>>(istream & i, Punct &p)
	{
		int a;
		char c;
		i>>c; // {
		while (c=='\n' || c==' ' || c=='>') i>>c;
		
		i>>a;
		p.setX(a);
		i>>c; // virgula
		i>>a;
		p.setY(a);
		i>>c; // }

		return i;
	}
	
	ostream &operator<<(ostream & o, const Punct &p)
	{
		o<<"{"<<p.getX()<<","<<p.getY()<<"}";
		return o;
	}
	*/