
#include "stdafx.h"

using namespace std;

class Process
{
public:
	Process(PROCESSENTRY32 data);

	PROCESSENTRY32 data;
};