#include <ostream>
#include <istream>
#include <string>
#include "shape.h"
#include "punct.h"
#include "Patrat.h"

using namespace std;

/*
template <class TIP> Patrat<TIP>::Patrat(const Punct<TIP> &a, const Punct<TIP> &b)
{
	this->p1.setX(a.getX());
	this->p1.setY(a.getY());

	this->p2.setX(b.getX());
	this->p2.setY(b.getY());
}
*/
/*
template <class TIP> Patrat<TIP>::~Patrat()
{
	//distructor
}
*/


/*
template <class TIP> void Patrat<TIP>::output(std::ostream &o) const
{
	o<<"P<"<<this->p1<<","<<this->p2<<">";
}

template <class TIP> void Patrat<TIP>::readFrom(std::istream &i)
{
	char c;

	i>>c; // <
	while (c=='\n' || c==' ' || c=='>') i>>c;

	if (c=='P') i>>c; // <

	i>>this->p1;
	i>>c;// ,
	i>>this->p2;
	i>>c;// >
}

template <class TIP> void Patrat<TIP>::moveBy(TIP a, TIP b)
{
	this->p1.moveBy(a,b);
	this->p2.moveBy(a,b);
}
*/


//ostream &operator<<(std::ostream &o, const Patrat &d)
//{
//	d.output(o);
//	return o;
//}