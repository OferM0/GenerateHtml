#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <windows.h>
#include <psapi.h>

#include "dllStruct.h"
#include "proccesStruct.h"
#include "sampleStruct.h"
#include "ProcessesInfo.h"
#pragma warning(disable:4996)

void getProccessName(TCHAR Buffer[MAX_PATH], char name[1000])
{
	char proccessName[1000];
	unsigned short determineChar;

	for (int i = 0; Buffer[i] != '\0'; i++) 
	{
		proccessName[i] = Buffer[i];
		determineChar = i + 1;
	}
	proccessName[determineChar] = '\0';

	/* to get tne name itself " ".exe
	
	for (int j = 0; proccessName[j] != '\0'; j++)
	{
		if (proccessName[j] == '\\') {
			determineChar = j+1;
		}
	}
	strcpy(proccessName, &proccessName[determineChar]);
	*/

	strcpy(name, proccessName);
}

void getDllName(TCHAR Buffer[MAX_PATH], char name[500])
{
	char dllName[500];
	unsigned short determineChar;

	for (int i = 0; Buffer[i] != '\0'; i++)
	{
		dllName[i] = Buffer[i];
		determineChar = i + 1;
	}
	dllName[determineChar] = '\0';

	/* to get tne name itself " ".dll

	for (int j = 0; dllName[j] != '\0'; j++)
	{
		if (dllName[j] == '\\') {
			determineChar = j + 1;
		}
	}
	strcpy(dllName, &dllName[determineChar]);
*/
	strcpy(name, dllName);
}

proccessList* PrintMemoryInfo(DWORD processID)
{
	proccessList* pList=NULL;
	char processName[1000], dllName[500], checkFirstDllEqualBuffer = 0;
	dllList* dlList=NULL;
	unsigned int dllCnt = 0;

	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);
	if (NULL == hProcess)
		return;

	HMODULE hMods[1024];
	DWORD cbNeeded;
	TCHAR Buffer[MAX_PATH];
	TCHAR Buffer2[MAX_PATH];

	if (GetModuleFileNameEx(hProcess, 0, Buffer, MAX_PATH))
	{
		// At this point, buffer contains the full path to the executable
		getProccessName(Buffer, processName);
	}
	else
	{
		// You better call GetLastError() here
	}

	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
	{
		
		// Get Dlls List

		if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
		{
			for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
			{
				TCHAR szModName[MAX_PATH];

				// Get the full path to the module's file.

				if (GetModuleFileNameEx(hProcess, hMods[i], Buffer2, MAX_PATH))
				{
					getDllName(Buffer2, dllName);
					if (checkFirstDllEqualBuffer != 0)
					{
						dlList = addDllItem(dllName);
						dllCnt++;
					}
					checkFirstDllEqualBuffer=1;
				}
			}
		}
		pList = addProccessItem(processName, processID, pmc.PageFaultCount, pmc.WorkingSetSize, pmc.QuotaPagedPoolUsage, pmc.QuotaPeakPagedPoolUsage, pmc.PagefileUsage, dlList, dllCnt);
		resetDllList();
		dlList = NULL;
	}
	CloseHandle(hProcess);
	return pList;
}

proccessList* GetProcessesInfo()
{
	proccessList* pList=NULL;

	// Get Processes
	// Receive all process ID

	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		// Error. Write to log
		return 1;
	}

	// Calculate how many process identifiers were returned.

	cProcesses = cbNeeded / sizeof(DWORD);

	// Print the memory usage for each process

	for (i = 0; i < cProcesses; i++)
	{
		pList = PrintMemoryInfo(aProcesses[i]);
	}

	// For each Process to get its Memory Information
	return pList;
}