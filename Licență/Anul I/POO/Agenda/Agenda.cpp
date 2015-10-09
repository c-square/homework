/*

--- The Agenda ---

aceasta clasa implementeaza urmatoarele
sabloane de programare:
	
	- observer: Clasa preia input-urile utilizatorului
		si notifica clasa Controller sa modifice starea
		copiilor, notificand (returnand o valoare) cand modificarea e gata

	- strategy: Utilizatorul are posibilitatea de a alege tipul algoritmului
		rulat si modul de executie al acestuia.





*/



#include <vector>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>


//my own
#include "sqlite3.h"

#ifndef structures_h
#define structures_h
#include "Structures.h"
#endif 

#ifndef common_h
#define common_h
#include "Common.h"
#endif 



#ifndef contact_model_h
#define contact_model_h
#include "ContactModel.h"
#endif 

#ifndef contact_view_h
#define contact_view_h
#include "ContactView.h"
#endif 



#ifndef acquaintance_model_h
#define acquaintance_model_h
#include "AcquaintanceModel.h"
#endif 

#ifndef collegue_model_h
#define collegue_model_h
#include "ColleagueModel.h"
#endif 

#ifndef friend_model_h
#define friend_model_h
#include "FriendModel.h"
#endif 





#ifndef agenda_view_h
#define agenda_view_h
#include "AgendaView.h"
#endif 

#ifndef agenda_h
#define agenda_h
#include "Agenda.h"
#endif 

#ifndef contact_controller_h
#define contact_controller_h
#include "ContactController.h"
#endif 





using namespace std;


//constructor
Agenda::Agenda()
{
		this->saved=false;

		this->Start();
}

bool Agenda::isSaved(){return this->saved;}
void Agenda::setSaved(){this->saved = true;}
void Agenda::unsetSaved(){this->saved = false;}


void Agenda::Start()
{
	int option;
	//see if there's a saved agenda
	msg("========= The Agenda ========\n");
	msgc("Welcome to the agenda");
	msgc("Please select your action");
	msgc("1. Load a saved agenda");
	msgc("2. Start a new agenda");
	msgc("3. Exit");

	cin>>option;
	switch (option)
	{
	case 1:
		this->setSaved();
		this->loadAgenda();
		break;
	case 2:
		break;
	case 3:
		return;
		break;
	default:
		break;
	}

	this->StartInterface();
}


void Agenda::StartInterface()
{
	int option;
	bool ok = true;

	msgc("........... Agenda Interface .............");
	while (ok)
	{
		this->printMenu();
		cin>>option;

		switch (option)
		{
		case 1:
			this->viewAllJustName();
			break;
		case 2:
			this->searchContact();
			break;
		case 3:
			this->filterContacts();
			break;
		case 4:
			this->addContact();
			this->unsetSaved();
			break;
		case 5:
			this->deleteContact();
			this->unsetSaved();
			break;
		case 6:
			this->editContact();
			this->unsetSaved();
			break;
		case 7:
			this->viewContact();
			this->unsetSaved();
			break;
		case 8:
			this->loadAgenda();
			break;
		case 9:
			this->saveAgenda();
			break;
		case 10:
			this->Close();
			ok = false;
			break;
		case 11:
			this->saveSqlAgenda();
			break;
		case 12:
			this->loadSqlAgenda();
			break;
		default:
			break;
		}
	}
}


//menu
void Agenda::printMenu()
{
	msg("------ The Menu:\n");
	msgc("1. View contacts");
	msgc("2. Search contacts");
	msgc("3. Filter contacts");
	msg("--------------------");
	msgc("4. Add new contact");
	msgc("5. Delete contact");
	msgc("6. Edit contact");
	msgc("7. View contact");
	msg("--------------------");
	msgc("8. Load from file");
	msgc("9. Save to file");
	msgc("10. Close agenda");
}

//views
void Agenda::viewAllJustName()
{
	int i, n;
	n=ContactController::getContactController()->nrContacts();

	msg("Format:  ID| NAME  SURNAME");
	for (i=0; i<n; i++)
	{
		cout<<"\t  "<<i<<"| "<<ContactController::getContactController()->getContactInfo(i, "nume")<<" "<<ContactController::getContactController()->getContactInfo(i, "prenume")<<"\n";
	}
}
void Agenda::searchContact()
{
	int opt, i, n;
	string search;

	msg("\n\n-- cautare:");
	msg("Choose search:");
	msgc("1.Nume");
	msgc("2.Prenume");
	msgc("3.Nr.Telefon");

	cin>>opt;

	msg("\nWrite search string:");
	cin>>search;

	msg("Format: ID| Nume Prenume : Telefon");
	
	n=ContactController::getContactController()->nrContacts();
	for (i=0; i<n; i++)
	{
		switch (opt)
		{
		case 1:
			if (ContactController::getContactController()->getContactInfo(i,"nume")==search)
			{
				this->printBasicContactInfo(i);
			}
			break;
		case 2:
			if (ContactController::getContactController()->getContactInfo(i,"prenume")==search)
			{
				this->printBasicContactInfo(i);
			}
			break;
		case 3:
			if (ContactController::getContactController()->getContactInfo(i,"telefon")==search)
			{
				this->printBasicContactInfo(i);
			}
			break;
		default:
			break;
		}
	}
}
void Agenda::filterContacts()
{
	int opt, i, n, opt2, opt3;
	string search;

	msg("\n\n-- cautare:");
	msg("Choose filter:");
	msgc("1.Tip");
	msgc("2.Id range");

	cin>>opt;

	switch (opt)
	{
	case 1:
		msg("Tipuri:");
		msg("1.Acquaintance");
		msg("2.Friend");
		msg("3.Colleague");
		msg("4.General contact");

		cin>>opt2;
		break;
	case 2:
		printf("Input interval(Max. contacts %d):", ContactController::getContactController()->nrContacts());
		cin>>opt2>>opt3;
		break;
	default:
		break;
	}

	msg("Format: ID| Nume Prenume : Telefon");
	
	n=ContactController::getContactController()->nrContacts();
	for (i=0; i<n; i++)
	{
		switch (opt)
		{
		case 1:
			switch (opt2)
			{
			case 1:
				if (ContactController::getContactController()->getContactInfo(i,"type")==TYPE_ACQUAINTANCE)
					this->printBasicContactInfo(i);
				break;
			case 2:
				if (ContactController::getContactController()->getContactInfo(i,"type")==TYPE_FRIEND)
					this->printBasicContactInfo(i);	
				break;
			case 3:
				if (ContactController::getContactController()->getContactInfo(i,"type")==TYPE_COLLEAGUE)
					this->printBasicContactInfo(i);
				break;
			case 4:
				if (ContactController::getContactController()->getContactInfo(i,"type")==TYPE_CONTACT)
					this->printBasicContactInfo(i);
				break;
			default:
				break;
			}
			
			break;
		case 2:
			if (i>=opt2 && i<=opt3)
			{
				this->printBasicContactInfo(i);
			}
			break;
		default:
			break;
		}
	}
}


void Agenda::printBasicContactInfo(int id)
{
	string nume = ContactController::getContactController()->getContactInfo(id, "nume");
	string prenume = ContactController::getContactController()->getContactInfo(id, "prenume");
	string tel = ContactController::getContactController()->getContactInfo(id, "telefon");

	cout<<"\t "<<id<<"|";
	cout<<"  "<<nume;
	cout<<"  "<<prenume<<" :";
	cout<<"  "<<tel<<"\n";
}

//load
void Agenda::loadAgenda()
{
	int n, i;
	ContactModel * c;

	string file, line;
	std::ifstream fin;


	//curata agenda
	this->CleanIfSaved();
	msg("Loading file");

	//start importing 
	msgc("File name:");

	cin>>file;
	fin.open(file.c_str());
	if (!fin)
	{
		msgc("Error opening file\n  Starting new agenda");
		return;
	}

	msgc("...Loading");
	fin>>n;
	for (i=0; i<n; i++)
	{
		fin>>line;
		c = ContactController::getContactController()->createContact(line);
		ContactController::getContactController()->addContact(c);
	}

	msgc("Agenda loaded!\n\n ====================");
	fin.close();
}
void Agenda::saveAgenda()
{
	string fileName;
	ofstream fout;
	int i, n;

	msg("\n\nPlease input agenda file name (with extension):");
	cin>>fileName;

	fout.open(fileName.c_str());
	
	n=ContactController::getContactController()->nrContacts();
	fout<<n<<"\n";
	for (i=0; i<n; i++)
	{
		fout<<ContactController::getContactController()->getContactInfoAsString(i);
		fout<<"\n";
	}

	msg ("Agenda saved\n\n");
}


void Agenda::loadSqlAgenda()
{
	sqlite3 *db;
	int rc;
	string sql = "SELECT * FROM agenda";
	sqlite3_stmt *stmHandle;

	const char *out;
	string value;
	string fileName;
	int i, n;

	msg("\n\nPlease input sql file name (with extension):");
	cin>>fileName;

	rc = sqlite3_open(fileName.c_str(), &db);
	sqlite3_prepare_v2(db, sql.c_str(), strlen(sql.data()), &stmHandle, &out);
	

	for (i=0; sqlite3_step(stmHandle) == SQLITE_ROW; i++)
	{
		printf("%16s", sqlite3_column_text(stmHandle, 1));
	}

	sqlite3_finalize(stmHandle);
	sqlite3_close(db);
	msg ("Agenda loaded\n\n");
}
void Agenda::saveSqlAgenda()
{
	sqlite3 *db;
	int rc;
	string sql = "CREATE TABLE agenda(id INTEGER PRIMARY KEY ASC, info TEXT)";
	sqlite3_stmt *stmHandle;

	const char *out;
	string fileName;
	int i, n;

	msg("\n\nPlease input sql file name (with extension):");
	cin>>fileName;

	rc = sqlite3_open(fileName.c_str(), &db);
	sqlite3_prepare(db, sql.c_str(), strlen(sql.data()), &stmHandle, &out);
	sqlite3_step(stmHandle);
	sqlite3_finalize(stmHandle);

	for (i=0; i<ContactController::getContactController()->nrContacts(); i++)
	{
		sql = "INSERT INTO agenda VALUES(NULL, ";
		sql.append(ContactController::getContactController()->getContactInfoAsString(i));
		sql.append(")");

		sqlite3_prepare(db, sql.c_str(), strlen(sql.data()), &stmHandle, &out);
		sqlite3_step(stmHandle);
		sqlite3_finalize(stmHandle);
	}

	sqlite3_close(db);
	msg ("Agenda saved\n\n");
}


//adds
void Agenda::addContact()
{
	vector<string> keys;
	ContactModel * p;
	string value, necesary;
	int n;
	
	this->unsetSaved();

	int i, opt1;
	msg("Add contact:");
	msgc("type:");
	msgc("1.Acquaintance");
	msgc("2.Friend");
	msgc("3.Colleague");
	msgc("4.Generic");

	cin>>opt1;
	switch (opt1)
	{
	case 1:
		p = AcquaintanceModel::CreateContact();
		
		keys = AcquaintanceModel::getNecesaryKeys();
		n = AcquaintanceModel::necesaryFieldsNumber();
		necesary = AcquaintanceModel::necesaryFields();

		msg("Necesary info in this order:");
		msg(necesary);
		for (i=0; i<n; i++)
		{
			cin>>value;
			p->addInfo(keys[i], value);
		}

		ContactController::getContactController()->addContact(p);
		break;
	case 2:
		p = FriendModel::CreateContact();
		
		keys = FriendModel::getNecesaryKeys();
		n = FriendModel::necesaryFieldsNumber();
		necesary = FriendModel::necesaryFields();

		msg("Necesary info in this order:");
		msg(necesary);
		for (i=0; i<n; i++)
		{
			cin>>value;
			p->addInfo(keys[i], value);
		}

		ContactController::getContactController()->addContact(p);
		break;
	case 3:
		p = ColleagueModel::CreateContact();
		
		keys = ColleagueModel::getNecesaryKeys();
		n = ColleagueModel::necesaryFieldsNumber();
		necesary = ColleagueModel::necesaryFields();

		msg("Necesary info in this order:");
		msg(necesary);
		for (i=0; i<n; i++)
		{
			cin>>value;
			p->addInfo(keys[i], value);
		}

		ContactController::getContactController()->addContact(p);
		break;
	case 4:
		p = ContactModel::CreateContact();
		
		keys = ContactModel::getNecesaryKeys();
		n = ContactModel::necesaryFieldsNumber();
		necesary = ContactModel::necesaryFields();

		msg("Necesary info in this order:");
		msg(necesary);
		for (i=0; i<n; i++)
		{
			cin>>value;
			p->addInfo(keys[i], value);
		}

		ContactController::getContactController()->addContact(p);
		break;
	default:
		break;
	}

	msg("Contact added\n\n");
}
void Agenda::deleteContact()
{
	int id;
	msg("Delete contact\n:Input id:");

	this->unsetSaved();

	cin>>id;
	ContactController::getContactController()->removeContact(id);

	msgc("Contact removed");
}
void Agenda::editContact()
{
	int id;
	string lines;
	msg("Edit contact:\n::Please input id::");

	cin>>id;
	lines = "temp";
	msg(lines);
	msg("Write the edit information one line at a time");
	msg("Format:  \"key:value\"");
	msg("Write \"exit\" to finish");

	while (1)
	{
		cin>>lines;
		if (lines == "exit")
			break;
		ContactController::getContactController()->modifyContact(id, lines);
	}

	msg("Contact modified");
	this->unsetSaved();
}
void Agenda::viewContact()
{
	int id, i;
	string info;
	vector<string> splitted;
	msg("View contact\n:Input id:");

	cin>>id;
	
	info = ContactController::getContactController()->getContactInfoAsString(id);
	splitted = split(info, OUTPUT_DELIMITER);

	for (i=0; i<splitted.size(); i++)
	{
		msgc(splitted[i]);
	}

}

//clean
void Agenda::CleanIfSaved()
{
	char c;

	if (!this->isSaved())
	{
		msg("\nThe current agenda is not saved. Do you want to save? y/n");
		cin>>c;

		if (c=='y')
			saveAgenda();
	}

	//clear
	ContactController::getContactController()->clear();
}


void Agenda::Close()
{
	char c;

	msg("Closing...");
	if (!this->isSaved())
	{
		msg("The agenda suffered modifications. Do you want to save? y/n");
		cin>>c;

		if (c=='y' || c=='Y')
			this->saveAgenda();
	}

	return;
}
//destructor
Agenda::~Agenda()
{
	ContactController::getContactController()->clear();
}


/*
sqlite

CREATE TABLE agenda(id INTEGER PRIMARY KEY ASC, info TEXT);
*/