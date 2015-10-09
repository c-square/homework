#include <ostream>
#include <istream>
#include <string>
#include <vector>

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


using namespace std;


//constructors
ContactModel::ContactModel() 
{
	this->addInfo("type", "");
	this->addInfo("nume", "");
	this->addInfo("prenume", "");
	this->addInfo("telefon", "");
}
ContactModel::ContactModel(vector<ExtraInfo> v) 
{
	int i;
	for (i=0; i<v.size(); i++)
		this->Info.insert(this->Info.end(), v[i]);
}


//common
void ContactModel::addInfo(string key, string value) 
{
	ExtraInfo temp;
	temp.key = key;
	temp.value = value;

	int i;
	for (i=0; i<this->Info.size(); i++)
		if (this->Info[i].key == key)
		{
			this->Info[i].value = value;
			break;
		}

	if (i==this->Info.size())
		this->Info.insert(this->Info.end(), temp);
}
void ContactModel::addInfo(string s) 
{
	ExtraInfo temp;
	vector<string> splitted = split(s, OUTPUT_DELIMITER);

	this->addInfo(splitted);
}
void ContactModel::addInfo(vector<string> v) 
{
	vector<string> splitted;
	int i;

	for (i=0; i<v.size(); i++)
	{
		splitted = split(v[i], KEY_DELIMITER);
		this->addInfo(splitted[0], splitted[1]);
	}
}
void ContactModel::addInfo(vector<ExtraInfo> v) 
{
	int i;
	for (i=0; i<v.size(); i++)
	{
		this->addInfo(v[i].key, v[i].value);
	}
}




void ContactModel::modifyInfo(string key, string value)
{
	int i;
	for (i=0; i<this->Info.size(); i++)
		if (this->Info[i].key == key)
		{
			this->Info[i].value = value;
			break;
		}
}
void ContactModel::modifyInfo(string s)
{
	vector<string> splitted;
	splitted = split(s, OUTPUT_DELIMITER);

	this->modifyInfo(splitted);
}
void ContactModel::modifyInfo(vector<string> v)
{
	vector<string> splitted;
	int i;

	for (i=0; i<v.size(); i++)
	{
		splitted = split(v[i], KEY_DELIMITER);
		this->modifyInfo(splitted[0], splitted[1]);
	}

}
void ContactModel::modifyInfo(vector<ExtraInfo> v)
{
	int i;

	for (i=0; i<v.size(); i++)
	{
		this->modifyInfo(v[i].key, v[i].value);
	}
}




void ContactModel::removeInfo(string key) 
{
	int i;
	for (i=0; i<this->Info.size(); i++)
		if (this->Info[i].key == key)
		{
			this->Info.erase(this->Info.begin()+i);
			break;
		}
}
string ContactModel::getInfo(string key)
{
	int i;
	for (i=0; i<this->Info.size(); i++)
		if (this->Info[i].key == key)
			return this->Info[i].value;
	return "";
}
string ContactModel::getAllInfoAsString()
{
	string str = ""; 
	int i;

	for (i=0; i<this->Info.size(); i++)
	{
		str.append(this->Info[i].key);
		str.append(KEY_DELIMITER_STR);
		str.append(this->Info[i].value);

		if (i!=this->Info.size()-1)
			str.append(OUTPUT_DELIMITER_STR);
	}

	return str;
}


vector<ExtraInfo>& ContactModel::getAllInfo()
{
	return this->Info;
}


bool ContactModel::ifHasValue(string key, string value)
{
	int i;
	string s = this->getInfo(key);

	if (s!="" && s==value)
		return true;
	return false;
}

//destructor
ContactModel::~ContactModel()
{
	this->Info.clear();
}



