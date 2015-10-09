

#include <ostream>
#include <istream>
#include <string>
#include <vector>

//my own
#ifndef structures_h
#define structures_h
#include "Structures.h"
#endif 

using namespace std;


class Contact
{
private:
	vector<ExtraInfo> Info;

public:
	Contact();

	void addInfo(string key, string value);
	void removeInfo(string key);
	void modifyInfo(string key, string value);
	string getInfo(string key);

	~Contact();
	
};


