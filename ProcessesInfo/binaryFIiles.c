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
#include "binaryFIiles.h"
#pragma warning(disable:4996)

proccessList* headProcess=NULL;
proccessList* tailProcess=NULL;

void saveInFile(t_headerFile fileHeader, sampleList* head)
{
	char fileName[100];
	sampleList* curr = head;
	proccessList* currProcess;
	dllList* currDll;
	printf("Enter file name:");
	scanf("%s", fileName);
	if (!curr)
	{
		//Log("The List is empty \n");
		return;
	}

	FILE* f = fopen(fileName, "wb");
	if (!f)
	{
		//LogError(strerror(GetLastError()));
		return;
	}

	fwrite(&fileHeader, sizeof(t_headerFile), 1, f);

	for (int i = 0; i < fileHeader.itemsCount; i++)
	{
		fwrite(curr, sizeof(sampleList), 1, f);

		currProcess = curr->pList;
		while (currProcess)
		{			
			fwrite(currProcess, sizeof(proccessList), 1, f);

			currDll = currProcess->dList;
			while (currDll)
			{
				fwrite(currDll, sizeof(dllList), 1, f);
				currDll = currDll->next;
			}
			//head->pList->dList = currDll;
			currProcess = currProcess->next;
		}
		//head->pList = currProcess;
		curr = curr->next;
	}
	//head = curr;
	fclose(f);
}

/*dllList* currentDll;
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
		free(releaseSample);*/