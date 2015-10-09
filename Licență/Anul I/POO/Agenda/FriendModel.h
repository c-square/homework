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




using namespace std;


class FriendModel: public ContactModel
{

public:
	FriendModel();

	~FriendModel();



	static int necesaryFieldsNumber()
	{
		return 4;
	}
	static string necesaryFields()
	{
		return string("\t1. Nume\n\t2. Prenume\n\t3. Nr.Tel.\n\t4. Birthday\n");
	}
	static vector<string> getNecesaryKeys()
	{
		vector<string> a;
		a.insert(a.end(), "nume");
		a.insert(a.end(), "prenume");
		a.insert(a.end(), "telefon");
		a.insert(a.end(), "workplace");

		return a;
	}


	static ContactModel* CreateContact()
	{
		ContactModel* p;
		p = new FriendModel();
		p->addInfo("type", TYPE_FRIEND);

		return p;
	}
};

