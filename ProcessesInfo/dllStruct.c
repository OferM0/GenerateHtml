#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <windows.h>
#include <psapi.h>
#include "dllStruct.h"
#pragma warning(disable:4996)

dllList* dllHead = NULL;
dllList* dllTail = NULL;

dllList* addDllItem(char dllName[500])
{
	dllList* curr;
	curr = (dllList*)malloc(sizeof(dllList));
	curr->next = NULL;
	curr->prev = NULL;

	strcpy(curr->dllName, dllName);

	if (dllHead == NULL)
	{
		dllHead = curr;
		dllTail = curr;
	}
	else
	{
		dllTail->next = curr;
		curr->prev = dllTail;
		dllTail = curr;
	}

	return dllHead;
}

int searchDllByName(char Name[500], dllList* search)
{
	dllList* curr = search;

	if (curr == NULL)
	{
		return 1;
	}

	while (curr != NULL)
	{
		if (strcmp(curr->dllName, Name)==0)
		{
			return 0;
		}
		curr = curr->next;
	}
	return 1;
}

void resetDllList()
{
	dllHead = dllTail = NULL;
}
