
#include <ostream>
#include <istream>
#include <string>
#include <vector>

//my own
#ifndef structures_h
#define structures_h
#include "Structures.h"
#endif 

#ifndef common_h
#define common_h
#include "Common.h"
#endif 




class ContactModel
{
private:
	vector<ExtraInfo> Info;
	string type;

public:
	ContactModel();
	ContactModel(vector<ExtraInfo>);


	void addInfo(string key, string value);
	void addInfo(string s);
	void addInfo(vector<string> v);
	void addInfo(vector<ExtraInfo> v);

	void modifyInfo(string key, string value);
	void modifyInfo(string s);
	void modifyInfo(vector<string> v);
	void modifyInfo(vector<ExtraInfo> v);

	string getInfo(string key);
	void removeInfo(string key);
	vector<ExtraInfo>& getAllInfo();
	string getAllInfoAsString();

	bool ifHasValue(string key, string value);

	//creator functions
	static ContactModel* CreateContact()
	{
		ContactModel* p;
		p = new ContactModel();
		p->addInfo("type", TYPE_CONTACT);

		return p;
	}


	static int necesaryFieldsNumber()
	{
		return 3;
	}
	static string necesaryFields()
	{
		return string("\t1. Nume\n\t2. Prenume\n\t3. Nr.Tel.\n");
	}
	static vector<string> getNecesaryKeys()
	{
		vector<string> a;
		a.insert(a.end(), "nume");
		a.insert(a.end(), "prenume");
		a.insert(a.end(), "telefon");

		return a;
	}


	~ContactModel();
};


