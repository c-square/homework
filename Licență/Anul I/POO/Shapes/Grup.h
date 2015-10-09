#ifndef GRUP_H__
#define GRUP_H__


#include <ostream>
#include <istream>
#include <string>
#include <vector>
#include "shape.h"
#include "punct.h"


template <class TIP> class Grup : public Shape<TIP>
{
private:	
	std::vector<Shape *> continut;
public:
	static const std::string identifier;
	Grup(){}
	~Grup(){}

	void add(Shape<TIP> *s)
	{
		this->continut.insert(this->continut.begin(), s);
	}

	void remove(Shape<TIP> *s)
	{
		unsigned int i;
		for (i=0; i<this->continut.size(); i++)
			if (s == this->continut[i])
			{
				this->continut.erase(this->continut.begin()+i);
				break;
			}
	}
	void output(std::ostream &o) const
	{
		unsigned int i;
		o<<"G{"<<this->continut.size()<<'\n';

		for (i=0; i<this->continut.size(); i++)
			o<<*(this->continut[i])<<'\n';
		o<<"}";
	}
	void readFrom(std::istream &i)
	{
		char c;
		Shape<TIP> *p;
		int n, nr;

		i>>c; // G
		if (c=='G') i>>c; //  {

		i>>n;
		//i>>c; // \n
		for (nr=0; nr<n; nr++)
		{
			i>>c;
			if (c=='T')
			{
				p = new Triunghi<TIP>();
				p->readFrom(i);
				//i>>c; // \n

				this->continut.insert(this->continut.begin()+nr, p);
			}
			else
			if (c=='D')
			{
				p = new Dreptunghi<TIP>();
				p->readFrom(i);
				//i>>c; // \n

				this->continut.insert(this->continut.begin()+nr, p);
			}
			else
			if (c=='C')
			{
				p = new Cerc<TIP>();
				p->readFrom(i);
				//i>>c; // \n

				this->continut.insert(this->continut.begin()+nr, p);
			}
			else
			if (c=='P')
			{
				p = new Patrat<TIP>();
				p->readFrom(i);
				//i>>c; // \n

				this->continut.insert(this->continut.begin()+nr, p);
			}
			else
			if (c=='G')
			{
				p = new Grup<TIP>();
				p->readFrom(i);
				//i>>c; // \n

				this->continut.insert(this->continut.begin()+nr, p);
			}

		}
		i>>c; // }\n
	}
	void moveBy(TIP a, TIP b)
	{
		unsigned int i;
		for (i=0; i<this->continut.size(); i++)
			this->continut[i]->moveBy(a,b);
	}


	friend std::ostream &operator<<(std::ostream &o, const Grup<TIP> &g)
	{
		unsigned int i;
		for (i=0; i<((Grup<TIP> *)(&g))->continut.size(); i++)
			o<<*(((Grup<TIP> *)(&g))->continut[i])<<'\n';

		return o;
	}
};


#endif