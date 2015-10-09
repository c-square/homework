
#include <ostream>
#include <istream>
#include <string>
#include <vector>

//my own
#ifndef structures_h
#define structures_h
#include "Structures.h"
#endif 

#ifndef contact_h
#define contact_h
#include "Contact.h"
#endif 


using namespace std;




	Contact::Contact()
	{
		this->addInfo("nume", "");
		this->addInfo("prenume","");
		this->addInfo("telefon", "");
	}

	void Contact::addInfo(string key, string value)
	{
		ExtraInfo temp;
		temp.key = key;
		temp.value = value;

		int i;
		for (i=0; i<this->Info.size(); i++)
		{
			if (this->Info[i].key == key)
			{
				this->Info[i].value = value;
				break;
			}
		}

		if (i==this->Info.size())
			this->Info.insert(this->Info.end(), temp);
	
	}
	void Contact::removeInfo(string key)
	{
		int i;
		for (i=0; i<this->Info.size(); i++)
		{
			if (this->Info[i].key == key)
			{
				this->Info.erase(this->Info.begin()+i);
				break;
			}
		}
	}
	void Contact::modifyInfo(string key, string value)
	{
		int i;
		for (i=0; i<this->Info.size(); i++)
		{
			if (this->Info[i].key == key)
			{
				this->Info[i].value = value;
				break;
			}
		}
	}
	string Contact::getInfo(string key)
	{
		int i;
		for (i=0; i<this->Info.size(); i++)
		{
			if (this->Info[i].key == key)
			{
				return this->Info[i].value;
				break;
			}
		}
	}

	Contact::~Contact()
	{
	
	}
	