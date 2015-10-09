#ifndef TRIUNGHI_H__
#define TRIUNGHI_H__


#include <ostream>
#include <string>
#include "shape.h"
#include "punct.h"


template <class TIP> class Triunghi : public Shape<TIP>
{
private:
	Punct<TIP> p1, p2, p3;
public:
	static const std::string identifier;

	Triunghi(const Punct<TIP> &a = Punct<TIP>(), const Punct<TIP> &b = Punct<TIP>(), const Punct<TIP> &c = Punct<TIP>())
	{
		this->p1.setX(a.getX());
		this->p1.setY(a.getY());

		this->p2.setX(b.getX());
		this->p2.setY(b.getY());

		this->p3.setX(c.getX());
		this->p3.setY(c.getY());
	}
	~Triunghi(){}

	
	void output(std::ostream &o) const
	{
		o<<"T<"<<this->p1<<","<<this->p2<<","<<this->p3<<">";
	}

	void readFrom(std::istream &i)
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


	void moveBy(TIP a, TIP b)
	{
		this->p1.moveBy(a,b);
		this->p2.moveBy(a,b);
		this->p3.moveBy(a,b);
	}

	
	static Shape* Create()
	{
		Triunghi<TIP> *p = new Triunghi<TIP>();
		return p;
	}


	friend std::ostream &operator<<(std::ostream &o, const Triunghi<TIP> &t)
	{
		t->output(o);
		return o;
	}
};



#endif