#include <ostream>
#include <string>
#include "shape.h"
#include "punct.h"
#include "triunghi.h"

using namespace std;

/*
template <class TIP> Triunghi<TIP>::Triunghi(const Punct<TIP> &a, const Punct<TIP> &b, const Punct<TIP> &c)
{
	this->p1.setX(a.getX());
	this->p1.setY(a.getY());

	this->p2.setX(b.getX());
	this->p2.setY(b.getY());

	this->p3.setX(c.getX());
	this->p3.setY(c.getY());
}*/
/*
template <class TIP> Triunghi<TIP>::~Triunghi()
{
	//destructor?
}

	




template <class TIP> void Triunghi<TIP>::output(std::ostream &o) const
{
	o<<"T<"<<this->p1<<","<<this->p2<<","<<this->p3<<">";
}

template <class TIP> void Triunghi<TIP>::readFrom(std::istream &i)
{
	char c;

	i>>c;   // <
	while (c=='\n' || c==' ' || c=='>') i>>c;

	if (c=='T') 
		i>>c;    // <

	i>>this->p1;
	i>>c;    // ,
	i>>this->p2;
	i>>c;    // ,
	i>>this->p3;
	i>>c;    // >
}


template <class TIP> void Triunghi<TIP>::moveBy(TIP a, TIP b)
{
	this->p1.moveBy(a,b);
	this->p2.moveBy(a,b);
	this->p3.moveBy(a,b);
}

*/

/*
ostream &operator<<(std::ostream &o, const Triunghi *t)
{
	t->output(o);
	return o;
}*/