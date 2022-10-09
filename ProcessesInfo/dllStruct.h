typedef struct dllList {

	//REAL DATA
	char dllName[500]; //dll name

	//STRUCT MANAGMENT
	struct dllList* next;
	struct dllList* prev;
}dllList;

dllList* addDllItem(char dllName[500]);

int searchDllByName(char Name[500], dllList* search);

void resetDllList();