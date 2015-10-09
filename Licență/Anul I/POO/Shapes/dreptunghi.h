#ifndef DREPTUNGHI_H__
#define DREPTUNGHI_H__


#include <ostream>
#include <istream>
#include <string>
#include "shape.h"
#include "punct.h"


template <class TIP> class Dreptunghi : public Shape<TIP>
{
private:
	Punct<TIP> p1, p2;	
public:
	static const std::string identifier;

	Dreptunghi(const Punct<TIP> &a = Punct<TIP>(), const Punct<TIP> &b = Punct<TIP>())
	{
		this->p1.setX(a.getX());
		this->p1.setY(a.getY());

		this->p2.setX(b.getX());
		this->p2.setY(b.getY());
	}

	~Dreptunghi(){}


	void output(std::ostream &o) const
	{
		o<<"D<"<<this->p1<<","<<this->p2<<">";
	}
	void readFrom(std::istream &i)
	{
		char c;

		i>>c; // <
		while (c=='\n' || c==' ' || c=='>') i>>c;

		if (c=='D') i>>c; // <

		i>>this->p1;
		i>>c;// ,
		i>>this->p2;
		i>>c;// >
	}
	void moveBy(TIP a, TIP b)
	{
		this->p1.moveBy(a,b);
		this->p2.moveBy(a,b);
	}




	static Shape<TIP>* Create()
	{
		Dreptunghi<TIP> *p = new Dreptunghi<TIP>();
		return p;
	}

	friend std::ostream &operator<<(std::ostream &o, const Dreptunghi<TIP> &d)
	{
		d.output(o);
		return o;
	}
};


#endif