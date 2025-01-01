#ifndef Header_H
#define Header_H
#include <stdio.h>
int NumRecordsFile;
int org;
int Inter;
int FB;
int Numberbloc;



typedef struct Record { //--------- Record structure---------//
    char data;       // Data
    int id;          // Bloc ID
    bool deleted;    // Logical deletion
} Record;

typedef struct Bloc { // -------------Bloc structure------//
    int id;          // Bloc ID
    Record* Data;   // Dynamic array of `Record`
    int next;        // Next Bloc ID or -1 if last Bloc
} Bloc;


typedef struct Meta { // Meta data structure fih les informations li 3andna 3la les fichiers
    char FileName[50];           // File name
    int FileId;
    int filesizeBloc;       // File size in Blocs
    int filesizeRecord;        // File size in Data
    int firstBlocaddress;   // Address of the first Bloc
    bool Global;             // Organization mode (1 = chained, 0 = contiguous)
    bool Intern;             // Sorted Data (1 = sorted, 0 = unsorted)
} Meta;

typedef struct MsHead { // Head block structure
    int* alloca;             // Allocation array
    int numberoffiles;       // Number of files

} MsHead;

typedef struct DataFile{
    char name[50];
    char* data;
    int id;
}DataFile;


void CreatMS(int FB, int NumberBloc, int NumberFile, int Org);
void initializeDisk(int* NumberBloc, int* FB, int* Org, int* inter);
void freeMS(FILE *MS, FILE *HEAD, FILE *META, int Org);
int* numFreeBlocs(FILE *HEAD, int* lenght);
void creatFile(FILE *MS, FILE*HEAD, FILE *META, DataFile File, int NumBlocsFile,int NumRecordsFile, int org, int Inter, int FB, int Numberbloc);
void renameFile(FILE *META, DataFile File, char NewName[50]);
bool allocateBlocs(FILE *MS, FILE*HEAD, FILE *META, DataFile File, int NumBlocsFile,int NumRecordsFile, int org, int Inter, int FB, int Numberbloc);
void freeBlocs(FILE *MS, FILE*HEAD, FILE *META, int org, int Inter, int FB, int* BlocsId);
void insertRecord(Meta* meta, Bloc* Bloc, int filenumber, char data, int* FB);
void searchRecord(MsHead* head, Meta* meta, Bloc* Bloc, int filenumber, int recordID, int* FB, int* blockNum, int* recordNum);
void deleteRecord(MsHead* head, Meta* meta, Bloc* Bloc, int filenumber, int recordID, int* FB);
void DeleteFile(char name[50], int numoffile, MsHead* head,  Meta* meta, Bloc* Bloc);
void PopulateFile(int filenumber, int numofrecord, MsHead* head, Meta* meta, Bloc* Bloc, int* FB);


#endif // Header_H