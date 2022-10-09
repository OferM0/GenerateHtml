#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <windows.h>
#include <psapi.h>
#include <time.h>

#include "dllStruct.h"
#include "proccesStruct.h"
#include "sampleStruct.h"
#include "ProcessesInfo.h"
#include "SnapShot.h"
#include "GenerateHtml.h"
#include "20SnapShot.h"
#pragma warning(disable:4996)

sampleList* TwentySnapShot()
{
	proccessList* pList=NULL;
	proccessList* addList=NULL;
	sampleList* sList;
	time_t t;
	char sampleTime[100];
	struct tm* timeinfo;
	unsigned char firstProccess = 0;

	time(&t);
	timeinfo = localtime(&t);

	sprintf(sampleTime, "%d.%d.%d %d:%d:%d\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	for (int i = 0; i <= 20; i++)
	{
		if (firstProccess == 0)
		{
			pList = GetProcessesInfo();
			resetProccessList();
			firstProccess++;
		}
		else {
			addList = GetProcessesInfo();
			resetProccessList();
			pList=SumProccesses(pList, addList);
			freeProccessMemory(addList);
		}
		Sleep(1000);
	}

	sampleCnt++;
	sList = addSampleItem(pList, sampleTime, sampleCnt);

	return sList;
}