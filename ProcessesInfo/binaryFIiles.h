typedef struct {
	int version;
	int itemsCount;
	char reserve[100];
} t_headerFile;

t_headerFile fileHeader;

void saveInFile(t_headerFile fileHeader, sampleList* head);