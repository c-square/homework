#ifndef SHAPE_H__
#define SHAPE_H__

#include <ostream>
#include <istream>


template <class TIP> class Shape
{
public:
	virtual void output(std::ostream &) const = 0{}
	virtual void moveBy(TIP, TIP) = 0{}
	virtual void readFrom(std::istream &) = 0{}
	virtual ~Shape() = 0{}
	
	friend std::ostream &operator<<(std::ostream &o, const Shape<TIP> &s)
	{
		s.output(o);
		return o;
	}
};


#endif