#include "stdafx.h"
#include "Process.h"
#include <vector>

typedef void(*IterProcessesCallback)(Process * proc, DWORD index, void * other);

using namespace std;

class ProcessPool
{
public:
	ProcessPool();
	void takeSnapshot();
	void iterProcesses(IterProcessesCallback callback, void * other);

	vector<Process> procList;
	HANDLE snapshotHandle;
};