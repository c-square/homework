#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h>

//my own
#ifndef common_h
#define common_h
#include "Common.h"
#endif 

#ifndef agenda_h
#define agenda_h
#include "Agenda.h"
#endif 



//std
using namespace std;

int initialize();

int main ()
{
	Agenda * agenda;

	//startup
	switch(initialize())
	{
	case 0:
		return 0;
		break;
	case 1:
		msg("Initialization done\n");
		agenda = new Agenda();
	default:
		break;
	}

	return 0;
}


int initialize()
{
	return 1;
}

