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


#ifndef acquaintance_model_h
#define acquaintance_model_h
#include "AcquaintanceModel.h"
#endif 

using namespace std;



AcquaintanceModel::AcquaintanceModel()
{
	this->addInfo("type", TYPE_FRIEND);
}

AcquaintanceModel::~AcquaintanceModel()
{
	return;
}


