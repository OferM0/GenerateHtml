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
#pragma warning(disable:4996)

sampleList* snapShot()
{
	proccessList* pList;
	sampleList* sList;
	time_t t;
	char userResponse, sampleTime[100];
	struct tm* timeinfo;

	time(&t);
	timeinfo = localtime(&t);

	sprintf(sampleTime, "%d.%d.%d %d:%d:%d\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	pList = GetProcessesInfo();
	resetProccessList();	
	sampleCnt++;
	sList=addSampleItem(pList, sampleTime, sampleCnt);

	return sList;
}