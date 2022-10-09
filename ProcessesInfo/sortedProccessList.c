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
#include "sortedProccessList.h"
#pragma warning(disable:4996)

proccessList* Head;
proccessList* Tail;

void replace(proccessList* List)
{
 
	if (List == NULL || List->next == NULL)
	{

	}
	else if (List == Head && List->next == Tail)
	{
		List->next = NULL;
		List->prev = Tail;
		Tail->prev = NULL;
		Tail->next = List;
		Tail = List;
		Head = List->prev;
	}
	else if (List == Head && List->next != NULL)
	{
		List->next = List->next->next;
		List->next->prev->next = List;
		List->next->prev->prev = NULL;
		List->prev = List->next->prev;
		List->next->prev = List;
		Head = List->prev;
	}
	else if (List->next == Tail)
	{
		List->prev->next = List->next;
		List->next->prev = List->prev;
		List->next->next = List;
		List->prev = List->next;
		List->next = NULL;
		Tail = List;
	}
	else {
		// regular swap
		List->prev->next = List->next;
		List->next->prev = List->prev;
		List->next->next->prev = List;
		List->prev = List->next;
		List->next = List->next->next;
		List->prev->next = List;
	}
}

proccessList* sortByDll(proccessList* pList)
{
	Head = pList;
	proccessList* curr = Head;

	while (curr!=NULL)
	{
		Tail = curr;
		curr = curr->next;
	}
	curr = Head;

	if (!curr) {
		return;
	}
	char change = 1;

	while (change != 0)
	{
		change = 0;
		while (curr != NULL)
		{
			if (curr->next != NULL && curr->dllCnt < curr->next->dllCnt)
			{
				replace(curr);
				change++;
			}
			curr = curr->next;
		}
		curr = Head;
	}
	return curr;
}

proccessList* makeCopySortedList(proccessList* pList)
{
	proccessList* curr;
	proccessList* copy = NULL;

	while (pList != NULL)
	{
		curr = (proccessList*)malloc(sizeof(proccessList));
		curr->next = NULL;
		curr->prev = NULL;

		strcpy(curr->processName, pList->processName);
		curr->proccessId = pList->proccessId;
		curr->PageFaultCount = pList->PageFaultCount;
		curr->WorkingSetSize = pList->WorkingSetSize;
		curr->QuotaPagedPoolUsage = pList->QuotaPagedPoolUsage;
		curr->QuotaPeakPagedPoolUsage = pList->QuotaPeakPagedPoolUsage;
		curr->PagefileUsage = pList->PagefileUsage;
		curr->dList = pList->dList;
		curr->dllCnt = pList->dllCnt;

		if (copy == NULL)
		{
			copy = curr;
		}
		else {
			while (copy->next != NULL)
			{
				copy = copy->next;
			}

			copy->next = curr;
			curr->prev = copy;
			copy = curr;
		}

		pList = pList->next;
	}

	while (copy->prev != NULL)
	{
		copy = copy->prev;
	}
	copy=sortByDll(copy);
}