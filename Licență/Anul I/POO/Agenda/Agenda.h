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


//my own
#ifndef structures_h
#define structures_h
#include "Structures.h"
#endif 

#ifndef common_h
#define common_h
#include "Common.h"
#endif 

#ifndef contact_controller_h
#define contact_controller_h
#include "ContactController.h"
#endif 

#ifndef contact_model_h
#define contact_model_h
#include "ContactModel.h"
#endif 

#ifndef contact_view_h
#define contact_view_h
#include "ContactView.h"
#endif 


#ifndef agenda_view_h
#define agenda_view_h
#include "AgendaView.h"
#endif 

#ifndef acquaintance_contact_view_h
#define acquaintance_contact_view_h
#include "AcquaintanceContactView.h"
#endif 



using namespace std;


class Agenda
{
private:
	bool saved;

public:
	Agenda();

	//save state
	bool isSaved();
	void setSaved();
	void unsetSaved();

	//start
	void Start();
	void StartInterface();

	//loading
	void loadAgenda();
	void saveAgenda();
	void loadSqlAgenda();
	void saveSqlAgenda();
	void printMenu();

	//views
	void viewAllJustName();
	void searchContact();
	void filterContacts();

	//addds
	void addContact();
	void deleteContact();
	void editContact();
	void viewContact();

	void printBasicContactInfo(int id);

	//cleanup
	void CleanIfSaved();

	void Close();
	~Agenda();
};