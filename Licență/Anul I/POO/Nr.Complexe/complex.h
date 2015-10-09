#include<iostream>
using namespace std;

#ifndef COMPLEX
#define COMPLEX

class Complex
{
	double re, im;				// partea reala si partea imaginara

public:    
	Complex(double=0, double=0);		// constructor

	double real() const;			// returneaza partea reala
	double imag() const;			// returneaza partea imaginara
	double modul() const;			// returneaza modulul (distanta fata de origine)
	
	void modificaRe(double=0);		// modifica partea reala
	void modificaIm(double=0);		// modifica partea imaginara
	void modifica(double=0, double=0);	// modifica partea reala si pe cea imaginara

	
	// returneaza distanta dintre obiectul curent si argument
	double distanta(const Complex&) const;

	// operatori supraincarcati
	Complex& operator+=(const Complex&);
	Complex& operator+=(double);
	Complex& operator*=(const Complex&);
	Complex& operator*=(double);
	
		
};


 static double distanta(const Complex&, const Complex&);

 ostream& operator<<(ostream& , const Complex& );
 istream& operator>>(istream&,  Complex&);

 const Complex operator+(const Complex&, const Complex&);
 const Complex operator*(const Complex&, double);
 const Complex operator*(double, const Complex&);

 bool  operator==(const Complex&, const Complex&);
 bool  operator!=(const Complex&, const Complex&);



#endif
