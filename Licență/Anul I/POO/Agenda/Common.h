#include <string>
#include <iostream>
#include <vector>
#include <istream>
#include <sstream>

using namespace std;


//outputsMessage
void msg(string str);
void msgc(string str);


//splitting functions
vector<string> &split(const string &s, char delim, vector<string> &elems);
vector<string> split(const string &s, char delim);