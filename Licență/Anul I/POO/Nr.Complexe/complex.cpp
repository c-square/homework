#include "complex.h"
#include <math.h>
#include <iostream>
using namespace std;


    // constructor
	Complex::Complex(double re, double im)
	{
		this->re = re;
		this->im = im;
	}


	double Complex::real() const {		return this->re;      } // returneaza partea reala
	double Complex::imag() const {      return this->im;      } // returneaza partea imaginara
	double Complex::modul() const {     return sqrt(re*re + im*im); } // returneaza modulul (distanta fata de origine)
	
	void Complex::modificaRe(double re){     this->re = re;        }		// modifica partea reala
	void Complex::modificaIm(double im){     this->im = im;        }		// modifica partea imaginara
	void Complex::modifica(double re, double im){    this->re = re;   this->im = im;     }	// modifica partea reala si pe cea imaginara

	
	
	// returneaza distanta dintre obiectul curent si argument
	double Complex::distanta(const Complex& a) const
	{
		return sqrt ((this->re - a.re)*(this->re - a.re) + (this->im - a.im)*(this->im - a.im));
	}

	// operatori supraincarcati
	Complex& Complex::operator+=(const Complex& a)
	{
		this->re += a.re;
		this->im += a.im;
		return *this;
	}

	Complex& Complex::operator+=(double a)
	{
		this->re += a;
		return *this;
	}

	Complex& Complex::operator*=(const Complex& a)
	{
		this->re = this->re*a.re - this->im*a.im;
		this->im = this->im*a.re + this->re*a.im;
		return *this;
	}

	Complex& Complex::operator*=(double a)
	{
		this->re *= a;
		this->im *= a;
		return *this;
	}
	
	
//out of the class
ostream& operator<<(ostream& o, const Complex& a)
{
	o<<'('<<a.real()<<','<<' '<<a.imag()<<')'<<' ';
	return o;
}

istream& operator>>(istream& i,  Complex& a)
{
	double re, im;
	i>>re>>im;
	a = Complex (re, im);
	return i;
}


static double distanta(const Complex& a, const Complex& b)
{
	return sqrt ((b.real() - a.real())*(b.real() - a.real()) + (b.imag() - a.imag())*(b.imag() - a.imag()));
}


const Complex operator+(const Complex& a, const Complex& b)
{
	return Complex(a.real()+b.real(), a.imag()+b.imag());
}

const Complex operator*(const Complex& a, double nr)
{
	return Complex(a.real()*nr, a.imag()*nr);
}

const Complex operator*(double nr, const Complex& a)
{
	return Complex(a.real()*nr, a.imag()*nr);
}



bool  operator==(const Complex& a, const Complex& b)
{
	return (a.real()==b.real())&&(a.imag()==b.imag());
}

bool  operator!=(const Complex& a, const Complex& b)
{
	return !((a.real()==b.real())&&(a.imag()==b.imag()));
}