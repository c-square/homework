/*
===============================================

-- Contact View --

- aceasta clasa implementeaza urmatoarele
sabloane de programare:

	-> Decorator Patter
		- impreuna cu celelalte view-uri

===============================================
*/




#include <istream>
#include <iostream>
#include <string>

//my own
#ifndef contact_model_h
#define contact_model_h
#include "ContactModel.h"
#endif 

#ifndef structures_h
#define structures_h
#include "Structures.h"
#endif 

#ifndef common_h
#define common_h
#include "Common.h"
#endif 

#ifndef contact_view_h
#define contact_view_h
#include "ContactView.h"
#endif 


class ColleagueContactView: public ContactView
{

public:
	void printContact(ContactModel &m);

	ostream& printContact(ContactModel &m, ostream& o);


	static ColleagueContactView* getContactView()
	{
		static ColleagueContactView instance;
		return &instance;
	}
};

