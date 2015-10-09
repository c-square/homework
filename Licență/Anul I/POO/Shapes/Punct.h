#ifndef PUNCT_H__
#define PUNCT_H__

#include <istream>
#include <ostream>


template <class TIP> class Punct
{
private:
	TIP x, y;

public:
	Punct(TIP a = 0, TIP b = 0)
	{
		this->x = a;
		this->y = b;
	}


	TIP getX() const {return this->x;}
	TIP getY() const {return this->y;}
	void setX(TIP a) {this->x = a;}
	void setY(TIP b) {this->y = b;}
	void moveBy(TIP a, TIP b) { this->x += a; this->y += b; }

	friend std::istream &operator>>(std::istream &i, Punct<TIP> &p)
	{
		TIP a;
		char c;
		i>>c; // {
		while (c=='\n' || c==' ' || c=='>') i>>c;
		
		i>>a;
		p.setX(a);
		i>>c; // virgula
		i>>a;
		p.setY(a);
		i>>c; // }

		return i;
	}
	friend std::ostream &operator<<(std::ostream &o, const Punct<TIP> &p)
	{
		o<<"{"<<p.getX()<<","<<p.getY()<<"}";
		return o;
	}
};


#endif