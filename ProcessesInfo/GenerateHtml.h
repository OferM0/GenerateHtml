void generateHtml(sampleList* sList);

SIZE_T getMemoryAvg(sampleList* sList);

unsigned short getDllCnt(sampleList* sList);

unsigned short getProccessCnt(proccessList* pList);

char* createLink(char arr[10], unsigned short cnt);

void createSampleHtml(sampleList* sList, char sampleHtmlName[20]);

dllList* removeMultiplies(sampleList* sList);

SIZE_T highestMemory(proccessList* pList);

void createSortSampleHtml(proccessList* pList, char sampleHtmlName[20], char sortSampleHtmlName[30]);

proccessList* removeProccessMultiplies(proccessList* pList);

proccessList* getProccessListByDllName(sampleList* sList, char dllName[500]);

void createDllHtml(sampleList* sList, char dllHtmlName[20], char dllName[20]);