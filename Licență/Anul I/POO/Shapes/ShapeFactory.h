#ifndef SHAPEFACTORY_H__
#define SHAPEFACTORY_H__


#include <istream>
#include <map>
#include <string>
#include "shape.h"


/* thrown when a shape cannot be read from a stream */

class WrongFormatException { };

template <class TIP> class ShapeFactory
{
	typedef Shape<TIP> *(createShapeFunction)(void);

public:
	
	static void registerFunction(const std::string &name, const createShapeFunction *fct)
	{
		ShapeFactory::getShapeFactory()->creationFunctions[name] = fct;
	}
	static Shape<TIP> *createShape(const std::string &str)
	{
		istringstream stream(str);
		Triunghi<TIP> *t;
		Cerc<TIP>* c;
		Dreptunghi<TIP> *d;
		Patrat<TIP> *p;
		Grup<TIP> *g;

		if (str[0]=='T')
		{
			t = new Triunghi<TIP>();
			t->readFrom(stream);
			return t;
		}
		else
		if (str[0]=='P')
		{
			t = new Patrat<TIP>();
			t->readFrom(stream);
			return t;
		}
		else
		if (temp=='G')
		{
			g = new Grup<TIP>();
			g->readFrom(i);
			return g;
		}
		else
		if (str[0]=='C')
		{
			c = new Cerc<TIP>();
			c->readFrom(stream);
			return c;
		}
		else
		if (str[0]=='D')
		{
			d = new Dreptunghi<TIP>();
			d->readFrom(stream);
			return d;
		}
	}
	static Shape<TIP> *createShape(std::istream &i)
	{
		Triunghi<TIP> *t;
		Cerc<TIP>* c;
		Dreptunghi<TIP> *d;
		Grup<TIP> *g;
		char temp;

		i>>temp;

		if (temp=='T')
		{
			t = new Triunghi<TIP>();
			t->readFrom(i);
			return t;
		}
		else
		if (temp=='C')
		{
			c = new Cerc<TIP>();
			c->readFrom(i);
			return c;
		}
		else
		if (temp=='D')
		{
			d = new Dreptunghi<TIP>();
			d->readFrom(i);
			return d;
		}
		else
		if (temp=='G')
		{
			g = new Grup<TIP>();
			g->readFrom(i);
			return g;
		}
		else
		if (temp=='P')
		{
			g = new Grup<TIP>();
			g->readFrom(i);
			return g;
		}
	}

private:
	std::map<std::string, createShapeFunction *> creationFunctions;
	ShapeFactory()
	{
		registerFunction("Cerc", &Cerc<TIP>::Create);
		registerFunction("Triunghi", &Triunghi<TIP>::Create);
		registerFunction("Dreptunghi", &Dreptunghi<TIP>::Create);
		registerFunction("Patrat", &Patrat<TIP>::Create);
	}
	static ShapeFactory<TIP> *getShapeFactory()
	{
		static ShapeFactory<TIP> instance;
        return &instance;
	}
};


#endif