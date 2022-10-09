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

void generateHtml(sampleList* sList)
{
    sampleList* original = sList;
    proccessList* currentProccess;    
    dllList* currentDll;

    if (sList != NULL)
    {
        currentProccess = sList->pList;
    }
    else
    {
        currentProccess = NULL;
    }

    char html[10000];
    char value[100];
    char link[2000];

    FILE* writeHome = fopen("index.html", "w");
    FILE* readTopHome = fopen("index1.txt", "r");
    FILE* readMiddleHome = fopen("index2.txt", "r");
    FILE* readBottomHome = fopen("index3.txt", "r");


    if ((!readTopHome) || (!readBottomHome) || (!readMiddleHome) || (!writeHome)) {
        printf("ERROR");//log
    }
    else {
        while (fgets(html, sizeof(html), readTopHome))
        {
            fputs(html, writeHome);
        }

        while (sList != NULL)
        {
            //SAMPLE LIST
            fputs("<tr>", writeHome);
            fputs("<td>", writeHome);
            sprintf(value, "%d", sList->sampleCnt);
            fputs(value, writeHome); //1
            fputs("</td>", writeHome);
            fputs("<td>", writeHome);
            sprintf(link, "%s%s%s%s%s", "<a class=\"link\" href=\"", createLink("sample", sList->sampleCnt), "\" target=\"_blank\">", createLink("sample", sList->sampleCnt), "</a>");
            fputs(link, writeHome);
            fputs("</td>", writeHome);
            sprintf(value, "%d", getProccessCnt(sList->pList));
            fputs("<td>", writeHome);
            fputs(value, writeHome); //3
            fputs("</td>", writeHome);
            fputs("<td>", writeHome);
            sprintf(value, "%d", getDllCnt(sList));
            fputs(value, writeHome); //4
            fputs("</td>", writeHome);
            fputs("<td>", writeHome);
            sprintf(value, "%zu", getMemoryAvg(sList));
            fputs(value, writeHome); //5
            fputs("</td>", writeHome);
            fputs("</tr>", writeHome);

            createSampleHtml(sList, createLink("sample", sList->sampleCnt));
            sList = sList->next;
        }

        sList = original;

        html[0]='\0';
        while (fgets(html, sizeof(html), readMiddleHome))
        {
            fputs(html, writeHome);
        }

        /* all dll list without removing multiplies
        * 
        while (sList != NULL)
        {
            currentProccess = sList->pList;
            //DLL'S LIST
            while (currentProccess != NULL)
            {
                currentDll = currentProccess->dList;

                while (currentDll != NULL)
                {
                    fputs("<tr>", writeHome);
                    fputs("<td>", writeHome);
                    fputs(currentDll->dllName, writeHome); //1
                    fputs("</td>", writeHome);
                    fputs("<td>", writeHome);
                    fputs("link", writeHome); //2
                    fputs("</td>", writeHome);
                    fputs("</tr>", writeHome);

                    currentDll = currentDll->next;
                }
                currentProccess = currentProccess->next;
            }
            sList = sList->next;
        }*/

        // current dll= new dll list based on dictionary.
        if (sList != NULL)
        {
            currentDll = removeMultiplies(sList);
            unsigned short dllNumber = 0;

            while (currentDll != NULL)
            {
                dllNumber++;

                fputs("<tr>", writeHome);
                fputs("<td>", writeHome);
                fputs(currentDll->dllName, writeHome); //1
                fputs("</td>", writeHome);
                fputs("<td>", writeHome);
                sprintf(link, "%s%s%s%s%s", "<a class=\"link\" href=\"", createLink("dll", dllNumber), "\" target=\"_blank\">", createLink("dll", dllNumber), "</a>");
                fputs(link, writeHome); //2
                fputs("</td>", writeHome);
                fputs("</tr>", writeHome);

                createDllHtml(sList, createLink("dll", dllNumber), currentDll->dllName);
                currentDll = currentDll->next;
            }
        }

        html[0] = '\0';
        while (fgets(html, sizeof(html), readBottomHome))
        {
            fputs(html, writeHome);
        }
    }
    fclose(readTopHome);
    fclose(readMiddleHome);
    fclose(readBottomHome);
    fclose(writeHome);
}

SIZE_T getMemoryAvg(sampleList* sList)
{
    SIZE_T memorySum = 0;
    unsigned short proccessCnt = 0;
    proccessList* currentProccess = sList->pList;

    while (currentProccess != NULL)
    {
        proccessCnt++;
        memorySum = memorySum + currentProccess->PageFaultCount + currentProccess->PagefileUsage + currentProccess->QuotaPagedPoolUsage + currentProccess->QuotaPeakPagedPoolUsage + currentProccess->WorkingSetSize;
        currentProccess = currentProccess->next;
    }

    if (proccessCnt != 0) { memorySum = memorySum / proccessCnt; }

    return memorySum;
}

unsigned short getDllCnt(sampleList* sList)
{
    unsigned short dllCnt = 0;
    proccessList* currentProccess = sList->pList;

    while (currentProccess != NULL)
    {
        dllCnt = dllCnt + currentProccess->dllCnt;
        currentProccess = currentProccess->next;
    }

    return dllCnt;
}

unsigned short getProccessCnt(proccessList* pList)
{
    unsigned short proccessCnt = 0;
    proccessList* currentProccess = pList;

    while (currentProccess != NULL)
    {
        proccessCnt++;
        currentProccess = currentProccess->next;
    }

    return proccessCnt;
}

char* createLink(char arr [10], unsigned short cnt)
{
    char* link = (char*)malloc(sizeof(char)* 20);
    sprintf(link, "%s%d.html", arr, cnt);
    return link;
}

char* createSortSampleHtmlLink(unsigned char cnt)
{
    char* link = (char*)malloc(sizeof(char) * 30);
    sprintf(link, "sortSample%d.html", cnt);
    return link;
}

void createSampleHtml(sampleList* sList, char sampleHtmlName[20])
{

    proccessList* currentProccess = sList->pList;
    dllList* currentDll;

    char html[10000];
    char sampleInformation[30];
    char value[100];
    char sortSampleHtmlName[30];

    FILE* writeSample = fopen(sampleHtmlName, "w");
    FILE* readSortBtn = fopen("sample0.txt", "r");
    FILE* readTopSample = fopen("sample1.txt", "r");
    FILE* readMiddleSample = fopen("sample2.txt", "r");
    FILE* readBottomSample = fopen("sample3.txt", "r");


    if ((!readTopSample) || (!readBottomSample) || (!readMiddleSample) || (!readSortBtn) || (!writeSample)) {
        printf("ERROR");//log
    }
    else {

        while (fgets(html, sizeof(html), readSortBtn))
        {
            fputs(html, writeSample);
        }

        strcpy(sortSampleHtmlName, createSortSampleHtmlLink(sList->sampleCnt));
        fputs(sortSampleHtmlName, writeSample); //

        html[0] = '\0';

        while (fgets(html, sizeof(html), readTopSample))
        {
            fputs(html, writeSample);
        }

        sprintf(sampleInformation, "%d at %s", sList->sampleCnt, sList->sampleTime);
        fputs(sampleInformation, writeSample); //

        html[0] = '\0';
        while (fgets(html, sizeof(html), readMiddleSample))
        {
            fputs(html, writeSample);
        }

        while (currentProccess != NULL)
        {
            //PROCCESS LIST
            fputs("<tr>", writeSample);
            fputs("<td>", writeSample);
            fputs(currentProccess->processName, writeSample); //1
            fputs("</td>", writeSample);
            fputs("<td>", writeSample);
            sprintf(value, "%d", currentProccess->proccessId);
            fputs(value, writeSample); //2
            fputs("</td>", writeSample);
            fputs("<td>", writeSample);
            sprintf(value, "%d", currentProccess->PageFaultCount);
            fputs(value, writeSample); //3
            fputs("</td>", writeSample);
            sprintf(value, "%zu", currentProccess->WorkingSetSize);
            if (highestMemory(sList->pList) == currentProccess->WorkingSetSize) // add icon
            {
                fputs("<td class=\"warning-icon\">", writeSample);
                fputs("<p>", writeSample);
                fputs(value, writeSample); //4
                fputs("<img class=\"warning-icon\" src=\"images/icons8-high-priority-64.png\"/></p>", writeSample); //1
            }
            else
            {
                fputs("<td>", writeSample);
                fputs(value, writeSample); //4
            }
            fputs("</td>", writeSample);
            fputs("<td>", writeSample);
            sprintf(value, "%zu", currentProccess->QuotaPagedPoolUsage);
            fputs(value, writeSample); //5
            fputs("</td>", writeSample);
            fputs("<td>", writeSample);
            sprintf(value, "%zu", currentProccess->QuotaPeakPagedPoolUsage);
            fputs(value, writeSample); //6
            fputs("</td>", writeSample);
            fputs("<td>", writeSample);
            sprintf(value, "%zu", currentProccess->PagefileUsage);
            fputs(value, writeSample); //7
            fputs("</td>", writeSample);
            fputs("<td>", writeSample);
            sprintf(value, "%d", currentProccess->dllCnt);
            fputs(value, writeSample); //8
            fputs("</td>", writeSample);

            fputs("<td>", writeSample); //dll's lists
            fputs("<div class = \"box\"><select>", writeSample);
            fputs("<option>List</option>", writeSample);
            currentDll = currentProccess->dList;
            while (currentDll != NULL)
            {
                fputs("<option>", writeSample);
                fputs(currentDll->dllName, writeSample);
                fputs("</option>", writeSample);
                currentDll = currentDll->next;
            }
            fputs("</select></div>", writeSample);
            fputs("</td>", writeSample);
            fputs("</tr>", writeSample);

            currentProccess = currentProccess->next;
        }

        html[0] = '\0';
        while (fgets(html, sizeof(html), readBottomSample))
        {
            fputs(html, writeSample);
        }

        currentProccess = sList->pList;
        createSortSampleHtml(currentProccess, sampleHtmlName, sortSampleHtmlName);
    }
    fclose(readTopSample);
    fclose(readMiddleSample);
    fclose(readBottomSample);
    fclose(writeSample);
    fclose(readSortBtn);
}

dllList* removeMultiplies(sampleList* sList)
{
    dllList* Check;
    dllList* dictionary = NULL;
    proccessList* currentProccess;
    dllList* curr;

    while (sList != NULL)
    {
        currentProccess = sList->pList;
        //DLL'S LIST
        while (currentProccess != NULL)
        {
            Check = currentProccess->dList;

            while (Check != NULL)
            {
                if (searchDllByName(Check->dllName, dictionary) != 0)
                {
                    curr = (dllList*)malloc(sizeof(dllList));
                    strcpy(curr->dllName, Check->dllName);
                    curr->next = NULL;
                    curr->prev = NULL;

                    if (dictionary == NULL)
                    {
                        dictionary = curr;
                    }
                    else {
                        while (dictionary->next != NULL)
                        {
                            dictionary = dictionary->next;
                        }

                        dictionary->next = curr;
                        curr->prev = dictionary;
                        dictionary = curr;
                    }

                    while (dictionary->prev != NULL)
                    {
                        dictionary = dictionary->prev;
                    }
                }
                Check = Check->next;
            }
            currentProccess = currentProccess->next;
        }
        sList = sList->next;
    }

    return dictionary;
}

SIZE_T highestMemory(proccessList* pList)
{
    SIZE_T cmp = 0;
    proccessList* currentProccess = pList;

    while (currentProccess != NULL)
    {
        if (currentProccess->WorkingSetSize > cmp)
        {
            cmp = currentProccess->WorkingSetSize;
        }
        currentProccess = currentProccess->next;
    }

    return cmp;
}

void createSortSampleHtml(proccessList* pList, char sampleHtmlName[20], char sortSampleHtmlName[30])
{
    SIZE_T mostMemory= highestMemory(pList);
    proccessList* original=pList;
    proccessList* currentProccess = makeCopySortedList(pList);
    dllList* currentDll;

    char html[10000];
    char value[100];

    FILE* writeSample = fopen(sortSampleHtmlName, "w");
    FILE* readSortBtn = fopen("sample0.txt", "r");
    FILE* readMiddleSample = fopen("sortSample1.txt", "r");
    FILE* readBottomSample = fopen("sample3.txt", "r");


    if ((!readSortBtn) || (!readBottomSample) || (!readMiddleSample) || (!writeSample)) {
        printf("ERROR");//log
    }
    else {

        while (fgets(html, sizeof(html), readSortBtn))
        {
            fputs(html, writeSample);
        }

        fputs(sampleHtmlName, writeSample); //

        html[0] = '\0';

        while (fgets(html, sizeof(html), readMiddleSample))
        {
            fputs(html, writeSample);
        }

        while (currentProccess != NULL)
        {
            //PROCCESS LIST
            fputs("<tr>", writeSample);
            fputs("<td>", writeSample);
            fputs(currentProccess->processName, writeSample); //1
            fputs("</td>", writeSample);
            fputs("<td>", writeSample);
            sprintf(value, "%d", currentProccess->proccessId);
            fputs(value, writeSample); //2
            fputs("</td>", writeSample);
            fputs("<td>", writeSample);
            sprintf(value, "%d", currentProccess->PageFaultCount);
            fputs(value, writeSample); //3
            fputs("</td>", writeSample);
            sprintf(value, "%zu", currentProccess->WorkingSetSize);
            if (mostMemory == currentProccess->WorkingSetSize) // add icon
            {
                fputs("<td class=\"warning-icon\">", writeSample);
                fputs("<p>", writeSample);
                fputs(value, writeSample); //4
                fputs("<img class=\"warning-icon\" src=\"images/icons8-high-priority-64.png\"/></p>", writeSample); //1
            }
            else
            {
                fputs("<td>", writeSample);
                fputs(value, writeSample); //4
            }
            fputs("</td>", writeSample);
            fputs("<td>", writeSample);
            sprintf(value, "%zu", currentProccess->QuotaPagedPoolUsage);
            fputs(value, writeSample); //5
            fputs("</td>", writeSample);
            fputs("<td>", writeSample);
            sprintf(value, "%zu", currentProccess->QuotaPeakPagedPoolUsage);
            fputs(value, writeSample); //6
            fputs("</td>", writeSample);
            fputs("<td>", writeSample);
            sprintf(value, "%zu", currentProccess->PagefileUsage);
            fputs(value, writeSample); //7
            fputs("</td>", writeSample);
            fputs("<td>", writeSample);
            sprintf(value, "%d", currentProccess->dllCnt);
            fputs(value, writeSample); //8
            fputs("</td>", writeSample);

            fputs("<td>", writeSample); //dll's lists
            fputs("<div class = \"box\"><select>", writeSample);
            fputs("<option>List</option>", writeSample);
            currentDll = currentProccess->dList;
            while (currentDll != NULL)
            {
                fputs("<option>", writeSample);
                fputs(currentDll->dllName, writeSample);
                fputs("</option>", writeSample);
                currentDll = currentDll->next;
            }
            fputs("</select></div>", writeSample);
            fputs("</td>", writeSample);
            fputs("</tr>", writeSample);

            currentProccess = currentProccess->next;
        }
        /*
        if (currentProccess != NULL)
        {
            //DLL'S LIST
            while (currentProccess != NULL)
            {
                currentDll = currentProccess->dList;

                while (currentDll != NULL)
                {
                    fputs("<tr>", writeSample);
                    fputs("<td>", writeSample);
                    fputs(currentDll->dllName, writeSample); //1
                    fputs("</td>", writeSample);
                    fputs("<td>", writeSample);
                    fputs("link", writeSample); //2
                    fputs("</td>", writeSample);
                    fputs("</tr>", writeSample);

                    currentDll = currentDll->next;
                }
                currentProccess = currentProccess->next;
            }
        }*/

        html[0] = '\0';
        while (fgets(html, sizeof(html), readBottomSample))
        {
            fputs(html, writeSample);
        }
    }
    fclose(readSortBtn);
    fclose(readMiddleSample);
    fclose(readBottomSample);
    fclose(writeSample);
}

/*
proccessList* removeProccessMultiplies(proccessList* pList)
{
    proccessList* dictionary = NULL;
    proccessList* currentProccess = pList;
    proccessList* curr;

    while (currentProccess != NULL)
    {

        if (searchProccessByName(currentProccess->processName, dictionary) != 0)
        {
            curr = (proccessList*)malloc(sizeof(proccessList));
            strcpy(curr->processName, currentProccess->processName);
            curr->next = NULL;
            curr->prev = NULL;

            if (dictionary == NULL)
            {
                dictionary = curr;
            }
            else {
                while (dictionary->next != NULL)
                {
                    dictionary = dictionary->next;
                }

                dictionary->next = curr;
                curr->prev = dictionary;
                dictionary = curr;
            }

            while (dictionary->prev != NULL)
            {
                dictionary = dictionary->prev;
            }
        }
        currentProccess = currentProccess->next;
    }

    return dictionary;
}*/

proccessList* getProccessListByDllName(sampleList* sList, char dllName[500])
{
    sampleList* currentSample=sList;
    proccessList* dictionary = NULL;
    proccessList* pList;
    proccessList* curr;
    dllList* Check;

    while (currentSample != NULL)
    {
        pList = currentSample->pList;
        //DLL'S LIST
        while (pList != NULL)
        {
            Check = pList->dList;

            while (Check != NULL)
            {
                if (strcmp(Check->dllName, dllName) == 0)
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

                    if (dictionary == NULL)
                    {
                        dictionary = curr;
                    }
                    else {
                        while (dictionary->next != NULL)
                        {
                            dictionary = dictionary->next;
                        }

                        dictionary->next = curr;
                        curr->prev = dictionary;
                        dictionary = curr;
                    }
                }
                Check = Check->next;
            }
            pList = pList->next;
        }
        currentSample = currentSample->next;
    }

    while (dictionary->prev != NULL)
    {
        dictionary = dictionary->prev;
    }

    return dictionary;
}


void createDllHtml(sampleList* sList, char dllHtmlName[20], char dllName[20])
{

    proccessList* currentProccess = /*removeProccessMultiplies*/(getProccessListByDllName(sList, dllName));
    dllList* currentDll;

    char html[10000];
    char value[300];

    FILE* writeDll = fopen(dllHtmlName, "w");
    FILE* readTopDll = fopen("Dll1.txt", "r");
    FILE* readMiddleDll = fopen("Dll2.txt", "r");
    FILE* readBottomDll = fopen("Dll3.txt", "r");


    if ((!readTopDll) || (!readBottomDll) || (!readMiddleDll) || (!writeDll)) {
        printf("ERROR");//log
    }
    else {
        while (fgets(html, sizeof(html), readTopDll))
        {
            fputs(html, writeDll);
        }

        sprintf(value, "%d PROCCESSES USED %s", getProccessCnt(getProccessListByDllName(sList, dllName)), dllName);
        fputs(value, writeDll); //

        html[0] = '\0';
        while (fgets(html, sizeof(html), readMiddleDll))
        {
            fputs(html, writeDll);
        }

        while (currentProccess != NULL)
        {
            //PROCCESS LIST
            fputs("<tr>", writeDll);
            fputs("<td>", writeDll);
            fputs(currentProccess->processName, writeDll); //1
            fputs("</td>", writeDll);
            fputs("<td>", writeDll);
            sprintf(value, "%d", currentProccess->proccessId);
            fputs(value, writeDll); //2
            fputs("</td>", writeDll);
            fputs("<td>", writeDll);
            sprintf(value, "%d", currentProccess->PageFaultCount);
            fputs(value, writeDll); //3
            fputs("</td>", writeDll);
            sprintf(value, "%zu", currentProccess->WorkingSetSize);
            fputs("<td>", writeDll);
            fputs(value, writeDll); //4
            fputs("</td>", writeDll);
            fputs("<td>", writeDll);
            sprintf(value, "%zu", currentProccess->QuotaPagedPoolUsage);
            fputs(value, writeDll); //5
            fputs("</td>", writeDll);
            fputs("<td>", writeDll);
            sprintf(value, "%zu", currentProccess->QuotaPeakPagedPoolUsage);
            fputs(value, writeDll); //6
            fputs("</td>", writeDll);
            fputs("<td>", writeDll);
            sprintf(value, "%zu", currentProccess->PagefileUsage);
            fputs(value, writeDll); //7
            fputs("</td>", writeDll);
            fputs("</tr>", writeDll);

            currentProccess = currentProccess->next;
        }

        html[0] = '\0';
        while (fgets(html, sizeof(html), readBottomDll))
        {
            fputs(html, writeDll);
        }
    }
    fclose(readTopDll);
    fclose(readMiddleDll);
    fclose(readBottomDll);
    fclose(writeDll);
}
