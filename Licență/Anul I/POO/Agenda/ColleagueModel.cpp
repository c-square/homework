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


#ifndef contact_model_h
#define contact_model_h
#include "ContactModel.h"
#endif 

#ifndef colleague_model_h
#define colleague_model_h
#include "ColleagueModel.h"
#endif 



using namespace std;


	ColleagueModel::ColleagueModel()
	{
		this->addInfo("type", TYPE_FRIEND);
	}

	ColleagueModel::~ColleagueModel()
	{
		return;
	}


