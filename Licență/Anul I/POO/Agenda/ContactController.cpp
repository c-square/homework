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



#ifndef acquaintance_contact_view_h
#define acquaintance_contact_view_h
#include "AcquaintanceContactView.h"
#endif 

#ifndef colleague_contact_view_h
#define colleague_contact_view_h
#include "ColleagueContactView.h"
#endif 

#ifndef friend_contact_view_h
#define friend_contact_view_h
#include "FriendContactView.h"
#endif 

#ifndef generic_contact_view_h
#define generic_contact_view_h
#include "GenericContactView.h"
#endif 






#ifndef acquaintance_model_h
#define acquaintance_model_h
#include "AcquaintanceModel.h"
#endif 

#ifndef colleague_model_h
#define colleague_model_h
#include "ColleagueModel.h"
#endif 

#ifndef friend_model_h
#define friend_model_h
#include "FriendModel.h"
#endif 



#ifndef contact_controller_h
#define contact_controller_h
#include "ContactController.h"
#endif 




//CMS Functions

//non-static
vector<ContactModel*>& ContactController::getContacts()
{
	return this->contacts;
}

//CMS
	//add
	void ContactController::addContact(ContactModel* c)
	{
		this->contacts.insert(this->contacts.end(), c);
	}
	ContactModel *ContactController::createContact(const string &str)
	{
		vector<string> info = split(str, OUTPUT_DELIMITER);
		return this->createContact(info);
	}
	ContactModel *ContactController::createContact(std::istream &i)
	{
		string info;
		i>>info;

		return ContactController::createContact(info);
	}
	ContactModel *ContactController::createContact(vector<string> info)
	{
		vector<string> splitted = split(info[0], KEY_DELIMITER);

		ContactModel* contact = ContactController::getContactController()->creationFunctions[splitted[1]]();
		contact->addInfo(info);
		return contact;
	}
	ContactModel *ContactController::createContact(vector<ExtraInfo> info)
	{
		ContactModel* contact = ContactController::getContactController()->creationFunctions[info[0].key]();
		contact->addInfo(info);
		return contact;
	}

	//remove
	void ContactController::removeContact(int id)
	{
		this->contacts.erase(this->contacts.begin()+id);
	}
	void ContactController::clear()
	{
		int i;
		for (i=0; i<this->contacts.size(); i++)
			removeContact(i);
	}

	//modify
	void ContactController::modifyContact(int id, string info)
	{
		this->contacts[id]->modifyInfo(info);
	}
	void ContactController::modifyContact(int id, vector<string> info)
	{
		this->contacts[id]->modifyInfo(info);
	}
	void ContactController::modifyContact(int id, vector<ExtraInfo> info)
	{
		this->contacts[id]->modifyInfo(info);
	}

	//view
	void ContactController::printContact(int id)
	{
		string type = this->contacts[id]->getInfo("type");

		if (type==TYPE_CONTACT)
			GenericContactView::getContactView()->printContact(*(this->contacts[id]));
		else
		if (type==TYPE_ACQUAINTANCE)
			AcquaintanceContactView::getContactView()->printContact(*(this->contacts[id]));
		else
		if (type==TYPE_COLLEAGUE)
			ColleagueContactView::getContactView()->printContact(*(this->contacts[id]));
		else
		if (type==TYPE_FRIEND)
			FriendContactView::getContactView()->printContact(*(this->contacts[id]));
		else
			GenericContactView::getContactView()->printContact(*(this->contacts[id]));
	}
	string ContactController::getContactInfo(int id, string info)
	{
		return this->contacts[id]->getInfo(info);
	}
	int ContactController::nrContacts()
	{
		return this->contacts.size();
	}
	string ContactController::getContactInfoAsString(int id)
	{
		return this->contacts[id]->getAllInfoAsString();
	}

	vector<int>& ContactController::searchContact(string key, string value)
	{
		vector<int> v;
		int i;
		for (i=0; i<this->contacts.size(); i++)
		{
			if ( this->contacts[i]->ifHasValue(key, value))
				v.insert(v.end(), i);
		}

		return v;
	}
		

	

ContactController::ContactController()
{
	registerFunction(TYPE_CONTACT, &ContactModel::CreateContact);
	registerFunction(TYPE_ACQUAINTANCE, &AcquaintanceModel::CreateContact);
	registerFunction(TYPE_COLLEAGUE, &ColleagueModel::CreateContact);
	registerFunction(TYPE_FRIEND, &FriendModel::CreateContact);
}
	