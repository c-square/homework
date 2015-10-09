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


#ifndef agenda_view_h
#define agenda_view_h
#include "AgendaView.h"
#endif 


using namespace std;



	AgendaView::AgendaView()
	{
	}
	AgendaView::~AgendaView(){}

	bool AgendaView::print(string s)
	{
		printf("%s", s.data());
		return true;
	}

