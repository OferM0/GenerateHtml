#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <windows.h>
#include <psapi.h>
#include "dllStruct.h"
#include "proccesStruct.h"
#include "sampleStruct.h"
#pragma warning(disable:4996)

sampleList* sampleHead = NULL;
sampleList* sampleTail = NULL;

sampleList* addSampleItem(proccessList* pList, char sampleTime[100], unsigned short sampleCnt)
{
	sampleList* curr;
	curr = (sampleList*)malloc(sizeof(sampleList));
	curr->next = NULL;
	curr->prev = NULL;

	curr->pList = pList;
	strcpy(curr->sampleTime, sampleTime);
	curr->sampleCnt = sampleCnt;

	if (sampleHead == NULL)
	{
		sampleHead = curr;
		sampleTail = curr;
	}
	else
	{
		sampleTail->next = curr;
		curr->prev = sampleTail;
		sampleTail = curr;
	}

	return sampleHead;
}

void resetSampleList()
{
	sampleHead = NULL;
	sampleTail = NULL;
}


void resetCollections()
{
	dllList* currentDll;
	dllList* releaseDll;
	proccessList* currentProccess;
	proccessList* releaseProccess;
	sampleList* currentSample = sampleHead;
	sampleList* releaseSample = sampleHead;

	while (currentSample != NULL)
	{
		currentProccess = currentSample->pList;
		releaseProccess = currentSample->pList;

		while (currentProccess != NULL)
		{
			currentDll = currentProccess->dList;
			releaseDll = currentProccess->dList;

			while (currentDll != NULL)
			{
				releaseDll = currentDll;
				currentDll = currentDll->next;
				free(releaseDll);
			}
			releaseProccess = currentProccess;
			currentProccess = currentProccess->next;
			free(releaseProccess);
		}
		releaseSample = currentSample;
		currentSample = currentSample->next;
		free(releaseSample);
	}
}