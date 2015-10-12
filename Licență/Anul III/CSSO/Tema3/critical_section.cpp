#include "stdafx.h"
using namespace std;

#define MIN_COUNT 1000

CRITICAL_SECTION gCS;
volatile int a;
volatile int b;

void ThreadGenereaza()
{
	for (int i = 0; i < MIN_COUNT; ++i)
	{
		EnterCriticalSection(&gCS);
		a = rand() % 10;
		b = rand() % 20;
		printf("a = %d, b = %d\n", a, b);
		LeaveCriticalSection(&gCS);
	}
}

void ThreadVerifica()
{
	for (int i = 0; i < MIN_COUNT; ++i)
	{
		EnterCriticalSection(&gCS);
		if (b == (2 * a))
			printf("corect\n");
		else
			printf("incorect\n");
		LeaveCriticalSection(&gCS);
	}
}

int main(int argc, _TCHAR* argv[])
{
	DWORD id_genereaza;
	DWORD id_verifica;
	HANDLE ht[2];

	InitializeCriticalSection(&gCS);

	printf("start\n");
	
	ht[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadGenereaza, (LPVOID)"genereaza", 0, &id_genereaza);
	assert(ht[0] != NULL);
	ht[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadVerifica, (LPVOID)"verifica", 0, &id_verifica);
	assert(ht[1] != NULL);
	WaitForMultipleObjects(2, ht, TRUE, INFINITE);
	CloseHandle(ht[0]);
	CloseHandle(ht[1]);

	printf("done\n");

	DeleteCriticalSection(&gCS);
	return 0;
}

