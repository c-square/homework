#include <iostream>
#include <vector>
#include "multime.h"
using namespace std;


	// constructor
	Multime::Multime(int nrElem, vector<int>& v)
	{
		this->nrElem = nrElem;
		this->v.swap(v);
	}


	Multime& Multime::operator+=(int a)
	{
		this->v.insert(this->v.end(), a);
		this->nrElem++;
		return *this;
	}

	Multime& Multime::operator-=(int a)
	{
		int i;
		for (i=0; i<this->nrElem; i++)
			if (this->v[i] == a)
			{
				this->v.erase(this->v.begin()+i);
				break;
			}
		this->nrElem--;
		return *this;
	}

	void Multime::operator=(Multime& a)
	{
		this->nrElem = a.getNrElem();
		this->v = a.getV();
	}


	vector<int>& Multime::getV()   {    return this->v;       }
	int Multime::getNrElem()	   {	return this->nrElem;  }
	void Multime::setNrElem(int a) {    this->nrElem = a; } 

	bool Multime::isInMultime(int a)
	{
		int i;
		for (i=0; i<nrElem; i++)
			if (v[i] == a)
				return 1;
		return 0;
	}




	//outside
	Multime operator+(Multime& a, Multime& b)
	{
		int i;

		Multime t;
		t=a;

		for (i=0; i<b.getNrElem(); i++)
		{
			if (!t.isInMultime(b.getV()[i]))
				t+=b.getV()[i];
		}

		return t;
	}


	bool operator==(Multime& a, Multime& b)
	{
		vector<int> carac1;
		vector<int> carac2;


		if (a.getNrElem() == b.getNrElem())
		{
			carac1 = a.getV();
			carac2 = b.getV();

			qsort (&carac1, a.getNrElem(), sizeof(int), Multime::compareFct);
			qsort (&carac2, b.getNrElem(), sizeof(int), Multime::compareFct);

			if (equal (carac1.begin(), carac1.end(), carac2.begin()))
				return 1;
		}
		return 0;
	}


	istream& operator>>(istream& i, Multime& t)
	{
		vector<int> a;
		int nr, j;
		int p;

		i>>nr;
		for (j=0; j<nr; j++)
		{
			i>>p;
			a.insert (a.end(), p);
		}

		t = Multime(nr, a);
		return i;

	}

	ostream& operator<<(ostream& o, Multime& t)
	{
		int i;
		vector<int>& p = t.getV();

		o<<"(";
		for (i=0; i<t.getNrElem(); i++)
		{
			o<<p[i];
			o<<",";
		}
		o<<")";

		return o;
	}

