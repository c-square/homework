#include "stdafx.h"
#include "Process.h"

using namespace std;

Process::Process(PROCESSENTRY32 data)
{
	memcpy(&(this->data), &data, sizeof(PROCESSENTRY32));
}

