typedef struct proccessList {

	//REAL DATA
	char processName[1000]; //proccess name
	DWORD proccessId; //proccess id

	//proccess memory data
	DWORD  PageFaultCount;
	SIZE_T WorkingSetSize;
	SIZE_T QuotaPagedPoolUsage;
	SIZE_T QuotaPeakPagedPoolUsage;
	SIZE_T PagefileUsage;

	//dll data
	dllList* dList;
	unsigned int dllCnt;

	//STRUCT MANAGMENT
	struct proccessList* next;
	struct proccessList* prev;
}proccessList;

proccessList* addProccessItem(char processName[1000], DWORD proccessId, DWORD  PageFaultCount, SIZE_T WorkingSetSize, SIZE_T QuotaPagedPoolUsage, SIZE_T QuotaPeakPagedPoolUsage, SIZE_T PagefileUsage, dllList* dList, unsigned int dllCnt);

void resetProccessList();

int searchProccessByName(char Name[1000], proccessList* search);

void  freeProccessMemory(proccessList* pList);

int searchProccessByNameAndId(char Name[1000], DWORD proccessId, proccessList* search);

proccessList* SumProccesses(proccessList* ret, proccessList* add);
