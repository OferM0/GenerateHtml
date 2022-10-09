typedef struct sampleList {

	//REAL DATA
	proccessList* pList; //proccess list

	//sample data
	char sampleTime[100];
	unsigned short sampleCnt;

	//STRUCT MANAGMENT
	struct sampleList* next;
	struct sampleList* prev;
}sampleList;

sampleList* addSampleItem(proccessList* pList, char sampleTime[100], unsigned short sampleCnt);

void resetSampleList();

void resetCollections();
