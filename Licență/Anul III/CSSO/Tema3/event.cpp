#include "stdafx.h"
using namespace std;

#define MIN_COUNT 1000

HANDLE generated, verified;
int a;
int b;

void ThreadGenereaza()
{
	for (int i = 0; i < MIN_COUNT; ++i)
	{
		WaitForSingleObject(verified, INFINITE);
		a = rand() % 10;
		b = rand() % 20;
		printf("a = %d, b = %d\n", a, b);
		ResetEvent(verified);
		SetEvent(generated);
	}
}

void ThreadVerifica()
{
	for (int i = 0; i < MIN_COUNT; ++i)
	{
		WaitForSingleObject(generated, INFINITE);
		if (b == (2 * a))
			printf("Corect\n");
		else
			printf("Incorect\n");
		ResetEvent(generated);
		SetEvent(verified);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	DWORD id_genereaza;
	DWORD id_verifica;
	HANDLE ht[2];

	generated = CreateEvent(NULL, TRUE, FALSE, (LPCWSTR)"generate_event");
	verified = CreateEvent(NULL, TRUE, TRUE, (LPCWSTR)"verify_event");

	if ((generated == NULL) || (verified == NULL))
	{
		printf("Error creating events!\n");
		return -1;
	}

	printf("start\n");

	ht[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadGenereaza, (LPVOID)"genereaza", 0, &id_genereaza);
	assert(ht[0] != NULL);
	ht[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadVerifica, (LPVOID)"verifica", 0, &id_verifica);
	assert(ht[1] != NULL);
	WaitForMultipleObjects(2, ht, TRUE, INFINITE);
	CloseHandle(ht[0]);
	CloseHandle(ht[1]);
	CloseHandle(generated);
	CloseHandle(verified);

	printf("done\n");
	scanf_s("%d", a);

	return 0;
}

