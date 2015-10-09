#ifndef CERC_H__
#define CERC_H__

#include <ostream>
#include <istream>
#include <string>
#include "shape.h"
#include "punct.h"



template <class TIP> class Cerc : public Shape<TIP>
{
private:
	Punct<TIP> c;
	TIP r;
public:
	static const std::string identifier;
	//constructor
	Cerc(const Punct<TIP> &p = Punct<TIP>(), const TIP r = 0)
	{
		this->c.setX(p.getX()); 
		this->c.setY(p.getY());

		this->r = r;
	}
	~Cerc(){}


	void output(std::ostream &o) const
	{
		o<<"C<"<<this->c<<",r:"<<this->r<<">";
	}

	void readFrom(std::istream &i)
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

	void moveBy(TIP a, TIP b)
	{
		this->c.moveBy(a,b);
	}


	static Shape* Create()
	{
		Cerc<TIP> *p = new Cerc<TIP>();
		return p;
	}

	friend std::ostream &operator<<(std::ostream &o, const Cerc<TIP> &c)
	{
		c.output(o);
		return o;
	}
};


#endif