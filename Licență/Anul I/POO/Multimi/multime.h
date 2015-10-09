#include <iostream>
#include <vector>
using namespace std;

#ifndef MULTIME
#define MULTIME

class Multime
{
	int nrElem;
	vector<int> v;

private:
	

public:

	Multime(int=0, vector<int>& = vector<int>());		// constructor
	~Multime(){}			// destructor

	vector<int>& getV();
	int getNrElem();
	void setNrElem(int);

	bool isInMultime(int a);

	static int compareFct(const void* a, const void* b)   // comparison function
	{
			int arg1 = *reinterpret_cast<const int*>(a);
			int arg2 = *reinterpret_cast<const int*>(b);
			if(arg1 < arg2) return -1;
			if(arg1 > arg2) return 1;
			return 0;
	}

	Multime& operator+=(int);
	Multime& operator-=(int);
	void operator=(Multime&);
};

Multime operator+( Multime&,  Multime&);
bool operator==(Multime&, Multime&);

istream& operator>>(istream& i, Multime& t);
ostream& operator<<(ostream& o, Multime& t);

#endif
