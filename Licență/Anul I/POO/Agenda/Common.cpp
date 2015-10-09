#include <string>
#include <iostream>
#include <vector>
#include <istream>
#include <sstream>


//my own


#ifndef agenda_view_h
#define agenda_view_h
#include "AgendaView.h"
#endif 

using namespace std;


//outputsMessage


void msg(string str)
{
	str.append("\n");
	AgendaView::getAgendaView()->print(str);
}

void msgc(string str)
{
	str.append("\n");
	AgendaView::getAgendaView()->print("->");
	AgendaView::getAgendaView()->print(str);

}

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}