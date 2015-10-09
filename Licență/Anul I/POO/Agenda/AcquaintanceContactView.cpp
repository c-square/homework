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

#ifndef acquaintance_contact_view_h
#define acquaintance_contact_view_h
#include "AcquaintanceContactView.h"
#endif 



void AcquaintanceContactView::printContact(ContactModel &m)
{
	vector<ExtraInfo> info = m.getAllInfo();
	int i;

	cout<<"Contact:{\n";
	cout<<"\tType"<<KEY_DELIMITER<<m.getInfo("type")<<"\n";

	for (i=0; i<info.size(); i++)
	{
		if(info[i].key != "type")
			cout<<"\t"<<info[i].key<<KEY_DELIMITER<<info[i].value<<"\n";
	}

	cout<<"}";
}

ostream& AcquaintanceContactView::printContact(ContactModel &m, ostream& o)
{
	vector<ExtraInfo> info = m.getAllInfo();
	int i;

		
	cout<<"type"<<KEY_DELIMITER<<m.getInfo("type")<<OUTPUT_DELIMITER;

	for (i=0; i<info.size(); i++)
	{
		if(info[i].key != "type")
			cout<<info[i].key<<KEY_DELIMITER<<info[i].value;

		if(i!=info.size()-1)
			cout<<OUTPUT_DELIMITER;
		else
			cout<<"\n";
	}

	return o;
}
	


	

