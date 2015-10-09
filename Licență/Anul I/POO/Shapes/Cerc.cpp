#include <ostream>
#include <istream>
#include <string>
#include "cerc.h"
#include "shape.h"
#include "punct.h"

using namespace std;

/*
template <class TIP> Cerc<TIP>::Cerc(const Punct<TIP> &p, const TIP r)
{
	this->c.setX(p.getX()); 
	this->c.setY(p.getY());

	this->r = r;
}*/

/*
template <class TIP> Cerc<TIP>::~Cerc()
{
	//destructor?
}




template <class TIP> void Cerc<TIP>::output(std::ostream &o) const
{
	o<<"C<"<<this->c<<",r:"<<this->r<<">";
}

template <class TIP> void Cerc<TIP>::readFrom(std::istream &i)
{
	char c;

	i>>c; // <
	while (c=='\n' || c==' ' || c=='>') i>>c;

	if (c=='C') i>>c; // <

	i>>this->c;
	i>>c;// ,
	i>>c;// r
	i>>c;// :
	i>>this->r;
	i>>c;// >
}

template <class TIP> void Cerc<TIP>::moveBy(TIP a, TIP b)
{
	this->c.moveBy(a,b);
}
*/

/*
ostream &operator<<(ostream &o, const Cerc &c)
{
	c.output(o);
	return o;
}
*/