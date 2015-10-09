#include <iostream>
#include <stdlib.h>
#include <vector>
#include "Punct.h"
#include "poligon.h"
#include "triunghi.h"
using namespace std;


int main ()
{
	Poligon p1, p2;
	vector<Punct> v;

	cout<<"Da-mi un poligon -\nNr. de puncte si apoi puncte:";
	cin>>p1;

	v.insert (v.end(), Punct(1,5));
	v.insert (v.end(), Punct(4,6));
	v.insert (v.end(), Punct(2,3));
	v.insert (v.end(), Punct(7,8));
	v.insert (v.end(), Punct(1,9));
	p2 = Poligon(5, v);

	cout<<"Poligonu 2"<<p2<<"\n";

	cout<<"Pol 1 convex: "<<p1.convex()<<"\n";
	cout<<"Pol 2 convex: "<<p2.convex()<<"\n";
	cout<<"Pol 1 arie: "<<p1.arie()<<"\n";
	cout<<"Pol 2 arie: "<<p2.arie()<<"\n";

	cin>>p1;

	return 0;
}