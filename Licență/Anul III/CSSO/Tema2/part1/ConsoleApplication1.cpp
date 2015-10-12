// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ProcessPool.h"
#include "usefull.h"

DWORD size;
DWORD finishValue = 0xFFFFFFFF;
DWORD invalid = 0x00000000;

TCHAR szName[] = TEXT("MyFileMappingObject");


void Privilege(TCHAR* privilege, BOOL bEnable)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;
	BOOL status;
	DWORD error;

	ZeroMemory(&tp, sizeof(tp));

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		printf("Failed at openProcessToken\n");
		return;
	}

	if (!LookupPrivilegeValue(NULL, privilege, &tp.Privileges[0].Luid))
	{
		printf("Failed at lookup privilege\n");
		CloseHandle(hToken);
		return;
	}

	tp.PrivilegeCount = 1;
	if (bEnable)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;
	status = AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, NULL);

	if (!status)
	{
		printf("Failed at adjust privilege\n");
		printLastError();
	}

	CloseHandle(hToken);
}

void processCallback(Process * p, DWORD procIndex, void * virtualFile)
{
	BYTE * fl = (BYTE *)virtualFile;

	if (p->data.cntThreads < 3)
	{
		printf("SELECTING PROCESS -> ");
		printf("%d\t", p->data.th32ProcessID);
		printf("%s\t", p->data.szExeFile);
		printf("Th: %d\t", p->data.cntThreads);
		printf("Parent: %d\t", p->data.th32ParentProcessID);
		printf("Priority: %dl\t", p->data.pcPriClassBase);
		printf("\n");

		memcpy((fl + procIndex * sizeof(DWORD)), &(p->data.th32ProcessID), sizeof(DWORD));
	}
	else
	{
		memcpy((fl + procIndex * sizeof(DWORD)), &invalid, sizeof(DWORD));
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	int i;
	void * address;
	HANDLE flHandle;
	ProcessPool pool;

	pool.takeSnapshot();

	size = (pool.procList.size() + 2) * (sizeof(DWORD));
	Privilege(TEXT("SeLockMemoryPrivilege"), TRUE);
	flHandle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, szName);
	if (flHandle)
	{
		address = MapViewOfFile(flHandle, FILE_MAP_ALL_ACCESS, 0, 0, size);
	
		pool.iterProcesses(processCallback, address);
		memcpy(((BYTE *)address + (pool.procList.size() + 1) * sizeof(DWORD)), &finishValue, sizeof(DWORD));

		cin >> i;
		Privilege(TEXT("SeLockMemoryPrivilege"), FALSE);
		CloseHandle(flHandle);
	}
	else
	{
		printLastError();
	}
	return 0;
}

