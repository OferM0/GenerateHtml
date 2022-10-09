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
#include "LongSnapShot.h"
#pragma warning(disable:4996)

sampleList* LongSnapShot()
{
	proccessList* pList = NULL;
	proccessList* addList = NULL;
	sampleList* sList;
	time_t t;
	char sampleTime[100];
	struct tm* timeinfo;
	unsigned char firstProccess = 0;
	int Stop = 0;
	char ch1[10];

	time(&t);
	timeinfo = localtime(&t);

	sprintf(sampleTime, "%d.%d.%d %d:%d:%d\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	while (!Stop)
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
			pList = SumProccesses(pList, addList);
			freeProccessMemory(addList);
		}
		Sleep(1000);
		if (kbhit())
		{
			gets(ch1);
			printf("Clicked %s\n", ch1);
			if (strcmp(ch1,"End")==0)
			{
				Stop = 1;
			}
		}
	}

	sampleCnt++;
	sList = addSampleItem(pList, sampleTime, sampleCnt);

	return sList;
}


