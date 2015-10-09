#include <fstream>
#include <vector>
#include "multime.h"
using namespace std;


int main ()
{
	Multime a, b, c;
	vector<int> v;

	cout<<"Da-mi un multime -\nNr. de elemente si apoi elementele:";
	cin>>a;

	v.insert (v.end(), 2);
	v.insert (v.end(), 3);
	v.insert (v.end(), 5);
	v.insert (v.end(), 6);
	v.insert (v.end(), 9);
	b = Multime(5, v);
	b+=4;
	b-=5;

	cout<<"Multimea 2"<<b<<"\n";

	c = (a+b);

	cout<<"Reuniune: "<<c<<"\n";
	cout<<"daca sunt Egale: "<<(a==b)<<"\n";
	
	cin>>a;

	return 0;
}