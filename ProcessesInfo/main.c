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
#define Version 1

int main()
{
	sampleList* sList;

	sampleCnt = 0;
	//sList = TwentySnapShot();
	//sList = LongSnapShot();
	sList = snapShot();
	generateHtml(sList);

	fileHeader.itemsCount = sampleCnt;
	fileHeader.version = 1;
	saveInFile(fileHeader, sList);

	resetCollections();
	resetSampleList();
	sList = NULL;
	generateHtml(sList);

	/*
	printf("Dear user, please choose what to do:\n\n1- Take One SnapShot.\n2- Take 60 SnapShots.\n3- Start Long SnapShot.\n4- End Long SnapShot.\n5- Generate HTML Report.\n6- Reset Collections.\n7- Save in file.\n8- Load from file.\n9-Quit.\n\n");
	scanf(" %c", &userResponse);
	printf("----------------------------------------------------------------------\n");
	while (userResponse != '9')
	{
		switch (userResponse)
		{
		case '1':

			break;

		case '2':

			break;

		case '3':

			break;

		case '4':

			break;

		case '5':

			break;

		case '6':

			break;

		case '7':

			break;

		case '8':

			break;

		default:
			printf("\nWrong selection. Try be more sharp\n");
			break;
		}

		printf("\nDear user, please choose what to do:\n\n1- Take One SnapShot.\n2- Take 60 SnapShots.\n3- Start Long SnapShot.\n4- End Long SnapShot.\n5- Generate HTML Report.\n6- Reset Collections.\n7- Save in file.\n8- Load from file.\n9-Quit.\n\n");
		scanf(" %c", &userResponse);
		printf("----------------------------------------------------------------------\n");
	}
	printf("Thank you for using our Great service. See you next time.\n");
*/
	return 0;
}