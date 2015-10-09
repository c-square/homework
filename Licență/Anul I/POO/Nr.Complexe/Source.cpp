#include "complex.h"
#include <fstream>
using namespace std;

int main ()
{
	Complex a,b,c;
	a.modificaRe (0.14);
	a.modificaIm (0.16);

	b.modifica (1.15, 2.15);
	c.modifica (5.15, 4.15);

	cout<<"Nr. initiale:\n";
	cout<<a<<' '<<a.modul()<<'\n';
	cout<<b<<' '<<b.modul()<<'\n';
	cout<<c<<' '<<c.modul()<<'\n';

	cout<<"Distanta de la a la b:"<<a.distanta(b)<<'\n';
	cout<<"Zi un nr. complex:\n";
	cin>>a;

	cout<<"Operatori:\n";
	cout<<"*= "<<(a*=b)<<'\n';
	cout<<"*=double "<<(a*=3)<<'\n';
	cout<<"+= "<<(a+=b)<<'\n';
	cout<<"+=double "<<(a+=4)<<'\n';
	cout<<"a==b: "<<(a==b)<<'\n';
	cout<<"a!=b: "<<(a!=b)<<'\n';

	cin>>a;
	return 0;
}

