/*
===============================================

-- Contact Controller --

- aceasta clasa implementeaza urmatoarele
sabloane de programare:

	-> Factory Method
	-> Composite Method
	-> Vizitor Patter = aceasta clasa extinde functionalitatea modelelor
						contact folosind diverse functii


===============================================
*/



#include <istream>
#include <fstream>
#include <map>
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

using namespace std;


class ContactController
{

public:

	typedef ContactModel *(createContactFunction)(void);
	
	//static
	static void registerFunction(const std::string &name, const createContactFunction *fct)
	{
		ContactController::getContactController()->creationFunctions[name] = fct;
	}

	//CMS Functions
	vector<ContactModel*> contacts;
	vector<ContactModel*>& getContacts();
	//CMS
		//add
		void addContact(ContactModel* c);
		ContactModel *createContact(const string &str);
		ContactModel *createContact(std::istream &i);
		ContactModel *createContact(vector<string> info);
		ContactModel *createContact(vector<ExtraInfo> info);

		//remove
		void removeContact(int id);
		void clear();

		//modify
		void modifyContact(int id, string info);
		void modifyContact(int id, vector<string> info);
		void modifyContact(int id, vector<ExtraInfo> info);

		//view
		void printContact(int id);
		string getContactInfo(int id, string info);
		int nrContacts();
		string getContactInfoAsString(int id);

		vector<int>& searchContact(string key, string value);
		

		static ContactController* getContactController()
		{
			static ContactController instance;
			return &instance;
		}

private:

	
	std::map<std::string, createContactFunction *> creationFunctions;

	ContactController();
	
};

