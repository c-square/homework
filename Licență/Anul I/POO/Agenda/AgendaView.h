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


using namespace std;


class AgendaView
{

public:
	AgendaView();
	~AgendaView();

	bool print(string s);



	static AgendaView* getAgendaView()
	{
		static AgendaView instance;
		return &instance;
	}
};