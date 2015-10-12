#include "stdafx.h"
#include "ProcessPool.h"
#include "usefull.h"

#include <vector>

using namespace std;

ProcessPool::ProcessPool()
{
	//do nothing cause life is better that way
}

void ProcessPool::takeSnapshot()
{
	PROCESSENTRY32 data;
	Process * pp;
	snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (snapshotHandle != INVALID_HANDLE_VALUE)
	{
		printf("\n====== PROCESSES =======\n\n");

		Process32First(snapshotHandle, &data);
		do
		{
			pp = new Process(data);
			this->procList.insert(this->procList.end(), *pp);

			printf("-> ");
			printf("%d\t", data.th32ProcessID);
			printf("%s\t", data.szExeFile);
			printf("Th: %d\t", data.cntThreads);
			printf("Parent: %d\t", data.th32ParentProcessID);
			printf("Priority: %dl\t", data.pcPriClassBase);
			printf("\n");

		} while (Process32Next(snapshotHandle, &data));
	}
	else
	{
		printLastError();
	}
}

void ProcessPool::iterProcesses(IterProcessesCallback callback, void * other)
{
	int i;
	for (i = 0; i < this->procList.size(); i++)
	{
		callback(&(this->procList[i]), i, other);
	}
}