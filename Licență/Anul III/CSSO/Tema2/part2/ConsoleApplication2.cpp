// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;
TCHAR szName[] = TEXT("MyFileMappingObject");

void printLastError()
{
	printf("\nError %d;\n", GetLastError());
}

BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(
		NULL,            // lookup privilege on local system
		lpszPrivilege,   // privilege to lookup 
		&luid))        // receives LUID of privilege
	{
		printf("LookupPrivilegeValue error: %u\n", GetLastError());
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = SE_PRIVILEGE_REMOVED;

	if (!AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL,
		(PDWORD)NULL))
	{
		printf("AdjustTokenPrivileges error: %u\n", GetLastError());
		return FALSE;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		printf("The token does not have the specified privilege. \n");
		return FALSE;
	}

	return TRUE;
}


DWORD killSystemPR(int pid)
{
	HANDLE prH;
	HANDLE myH;
	HANDLE token;
	DWORD code;
	TOKEN_INFORMATION_CLASS info;
	TOKEN_PRIVILEGES privileges;
	LUID luid;

	myH = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_SET_INFORMATION, true, GetCurrentProcessId());
	if (myH != INVALID_HANDLE_VALUE)
	{
		if (OpenProcessToken(myH, TOKEN_ADJUST_PRIVILEGES, &token))
		{
			SetPrivilege(token, SE_TCB_NAME, true);

			prH = OpenProcess(PROCESS_TERMINATE, false, pid);
			if (prH != INVALID_HANDLE_VALUE && prH != 0)
			{
				GetExitCodeProcess(prH, &code);
				TerminateProcess(prH, code);
			}
			else
				return -3;
		}
		else
			return -2;
	}
	else
		return -1;

	return 1;
}

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

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		printf("The token does not have the specified privilege. \n");
	}

	CloseHandle(hToken);
}

int _tmain(int argc, _TCHAR* argv[])
{
	int i;
	void * address;
	HANDLE fh;
	DWORD buffer;
	DWORD val;

	Privilege(SE_TCB_NAME, TRUE);
	Privilege(SE_DEBUG_NAME, TRUE);

	//GET THE MEMORY ZONE TO READ FROM
	fh = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, szName);
	if (fh)
	{
		address = MapViewOfFile(fh, FILE_MAP_ALL_ACCESS, 0, 0, 1024);
		
		for (i = 0;; i++)
		{
			buffer = *((DWORD *)address + i);
			if (buffer == 0xffffffff) break;
			if (buffer == 0x0) continue;

			printf("Found PID: %d\n", buffer);
			val = killSystemPR(buffer);
			if (val != 1)
			{
				printf("Val: %d\n", val);
				printLastError();
			}
		}
	}
	else
	{
		printf("Failed on OpenFileMapping\n");
		printLastError();
	}

	cin >> i;
	return 0;
}

