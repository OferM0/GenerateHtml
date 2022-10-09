#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <windows.h>
#include <psapi.h>
#include "dllStruct.h"
#include "proccesStruct.h"
#pragma warning(disable:4996)

proccessList* proccessHead = NULL;
proccessList* proccessTail = NULL;

 proccessList* addProccessItem(char processName[1000], DWORD proccessId, DWORD  PageFaultCount, SIZE_T WorkingSetSize, SIZE_T QuotaPagedPoolUsage, SIZE_T QuotaPeakPagedPoolUsage, SIZE_T PagefileUsage, dllList* dList, unsigned int dllCnt)
 {
	 proccessList* cur;
	 cur=(proccessList*)malloc(sizeof(proccessList));
	 cur->next = NULL;
	 cur->prev = NULL;

	 strcpy(cur->processName, processName);
	 cur->proccessId = proccessId;
	 cur->PageFaultCount = PageFaultCount;
	 cur->WorkingSetSize = WorkingSetSize;
	 cur->QuotaPagedPoolUsage = QuotaPagedPoolUsage;
	 cur->QuotaPeakPagedPoolUsage = QuotaPeakPagedPoolUsage;
	 cur->PagefileUsage = PagefileUsage;
	 cur->dList = dList;
	 cur->dllCnt = dllCnt;

	 if (proccessHead == NULL)
	 {
		 proccessHead = cur;
		 proccessTail = cur;
	 }
	 else
	 {
		 proccessTail->next = cur;
		 cur->prev = proccessTail;
		 proccessTail = cur;
	 }

	 return proccessHead;
 }
 
 int searchProccessByName(char Name[1000], proccessList* search)
 {
	 proccessList* curr = search;

	 if (curr == NULL)
	 {
		 return 1;
	 }

	 while (curr != NULL)
	 {
		 if (strcmp(curr->processName, Name) == 0)
		 {
			 return 0;
		 }
		 curr = curr->next;
	 }
	 return 1;
 }

 void resetProccessList()
{
	proccessHead = proccessTail = NULL;
}

 void freeProccessMemory(proccessList* pList)
 {
	 dllList* currentDll;
	 dllList* releaseDll;
	 proccessList* currentProccess=pList;
	 proccessList* releaseProccess=pList;

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
 }

 int searchProccessByNameAndId(char Name[1000], DWORD proccessId, proccessList* search)
 {
	 proccessList* curr = search;

	 if (curr == NULL)
	 {
		 return 1;
	 }

	 while (curr != NULL)
	 {
		 if (strcmp(curr->processName, Name) == 0 && proccessId==curr->proccessId)
		 {
			 return 0;
		 }
		 curr = curr->next;
	 }
	 return 1;
 }

 proccessList* SumProccesses(proccessList* ret, proccessList* add)
 {
	 proccessList* currentProccess;
	 proccessList* retTail = ret;
	 proccessList* retHead = ret;
	 proccessList* cur;
	 dllList* curr;
	 dllList* retDll;
	 dllList* addDll;
	 dllList* newProccesDll;
	 dllList* newProccesDllList=NULL;

	 while (retHead->prev != NULL)
	 {
		 retHead = retHead->prev;
	 }
	 while (retTail->next != NULL)
	 {
		 retTail = retTail->next;
	 }

	 while (add != NULL)
	 {
		 currentProccess = retHead;

		 if (searchProccessByNameAndId(add->processName, add->proccessId, currentProccess) != 0)
		 {
			 cur = (proccessList*)malloc(sizeof(proccessList));
			 cur->next = NULL;
			 cur->prev = NULL;

			 strcpy(cur->processName, add->processName);
			 cur->proccessId = add->proccessId;
			 cur->PageFaultCount = add->PageFaultCount;
			 cur->WorkingSetSize = add->WorkingSetSize;
			 cur->QuotaPagedPoolUsage = add->QuotaPagedPoolUsage;
			 cur->QuotaPeakPagedPoolUsage = add->QuotaPeakPagedPoolUsage;
			 cur->PagefileUsage = add->PagefileUsage;
			 cur->dllCnt = add->dllCnt;

			 //build dllList for the new proccess 
			 addDll = add->dList;
			 while (addDll != NULL)
			 {
				 newProccesDll = (dllList*)malloc(sizeof(dllList));
				 strcpy(newProccesDll->dllName, addDll->dllName);
				 newProccesDll->next = NULL;
				 newProccesDll->prev = NULL;

				 if (newProccesDllList == NULL)
				 {
					 newProccesDllList = newProccesDll;
				 }
				 else {
					 while (newProccesDllList->next != NULL)
					 {
						 newProccesDllList = newProccesDllList->next;
					 }

					 newProccesDllList->next = newProccesDll;
					 newProccesDll->prev = newProccesDllList;
					 newProccesDllList = newProccesDll;
				 }

				 /*while (newProccesDllList->prev != NULL)
				 {
					 newProccesDllList = newProccesDllList->prev;
				 }*/

				 addDll = addDll->next;
			 }
			 if (newProccesDllList != NULL)
			 {
				 while (newProccesDllList->prev != NULL)
				 {
					 newProccesDllList = newProccesDllList->prev;
				 }
			 }
			 cur->dList = newProccesDllList;
			 newProccesDllList = NULL;

			 if (retHead == NULL)
			 {
				 retHead = cur;
				 retTail = cur;
			 }
			 else
			 {
				 retTail->next = cur;
				 cur->prev = retTail;
				 retTail = cur;
			 }
			 currentProccess = retHead;
		 }
		 else if(searchProccessByNameAndId(add->processName, add->proccessId, currentProccess) == 0)
		 {

			 while (currentProccess != NULL)
			 {
				 if (strcmp(currentProccess->processName, add->processName) == 0 && currentProccess->proccessId == add->proccessId)
				 {
					 retDll = currentProccess->dList;
					 addDll = add->dList;

					 while (addDll != NULL)
					 {
						 if (searchDllByName(addDll->dllName, retDll) != 0)
						 {
							 currentProccess->dllCnt++;
							 curr = (dllList*)malloc(sizeof(dllList));
							 strcpy(curr->dllName, addDll->dllName);
							 curr->next = NULL;
							 curr->prev = NULL;

							 if (retDll == NULL)
							 {
								 retDll = curr;
							 }
							 else {
								 while (retDll->next != NULL)
								 {
									 retDll = retDll->next;
								 }

								 retDll->next = curr;
								 curr->prev = retDll;
								 retDll = curr;
							 }

							 while (retDll->prev != NULL)
							 {
								 retDll = retDll->prev;
							 }
						 }
						 addDll = addDll->next;
					 }

					 currentProccess->PageFaultCount += add->PageFaultCount;
					 currentProccess->WorkingSetSize += add->WorkingSetSize;
					 currentProccess->QuotaPagedPoolUsage += add->QuotaPagedPoolUsage;
					 currentProccess->QuotaPeakPagedPoolUsage += add->QuotaPeakPagedPoolUsage;
					 currentProccess->PagefileUsage += add->PagefileUsage;
				 } 
				 currentProccess = currentProccess->next;
			 }	 
		 }
		 add = add->next;
	 }

	 return retHead;
 }