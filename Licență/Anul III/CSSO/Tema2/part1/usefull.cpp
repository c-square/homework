#include "stdafx.h"
#include "usefull.h"
#include <stdio.h>

void printLastError()
{
	printf("\nError %d;\n", GetLastError());
}