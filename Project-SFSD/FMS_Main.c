
#include "../include/raylib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

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


//NumberBloc is the number of blocs, FB is the number of records in each bloc, Org is the Orgnization mode, 
//inter is the sorting mode and vector is the allocation table




/*-------------------------------------- FAHD PART (AND WASSIM )---------------------------------------------------*/


//updating and saving changes to the main memory
void saveMs(FILE *MS, FILE *HEAD, FILE *META, FILE *Main_Memory, int NumFiles, int saveChoice){
    rewind(MS);
    rewind(HEAD);
    rewind(META);

    Bloc BlocBuffer;
    Meta MetaBuffer;
    MsHead HeadBuffer;

    switch(saveChoice){
        case 1:
            rewind(MS);
            rewind(HEAD);
            rewind(META);

            fseek(Main_Memory, 0, SEEK_SET);
            fread(&HeadBuffer, sizeof(HeadBuffer), 1, HEAD);
            fwrite(&HeadBuffer, sizeof(HeadBuffer), 1, Main_Memory);
            break;

        case 2:
            rewind(MS);
            rewind(HEAD);
            rewind(META);

            fseek(Main_Memory, sizeof(HeadBuffer) + sizeof(MetaBuffer), SEEK_SET);
            while(fread(&MetaBuffer, sizeof(MetaBuffer), 1, META)){
                fwrite(&MetaBuffer, sizeof(MetaBuffer), 1, Main_Memory);
            }

            break;

        case 3:
            rewind(MS);
            rewind(HEAD);
            rewind(META);

            fseek(Main_Memory, sizeof(HeadBuffer) + sizeof(MetaBuffer) * NumFiles + sizeof(BlocBuffer) , SEEK_SET);
            while(fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS)){
                fwrite(&BlocBuffer, sizeof(BlocBuffer), 1, Main_Memory);
            }
            break;

        case 4:
            rewind(MS);
            rewind(HEAD);
            rewind(META);

            fread(&HeadBuffer, sizeof(HeadBuffer), 1, HEAD);
            fwrite(&HeadBuffer, sizeof(HeadBuffer), 1, Main_Memory);

            while(fread(&MetaBuffer, sizeof(MetaBuffer), 1, META)){
                fwrite(&MetaBuffer, sizeof(MetaBuffer), 1, Main_Memory);
            }

            while(fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS)){
                fwrite(&BlocBuffer, sizeof(BlocBuffer), 1, Main_Memory);
            }
            break;
        default :
            rewind(MS);
            rewind(HEAD);
            rewind(META);

            fread(&HeadBuffer, sizeof(HeadBuffer), 1, HEAD);
            fwrite(&HeadBuffer, sizeof(HeadBuffer), 1, Main_Memory);

            while(fread(&MetaBuffer, sizeof(MetaBuffer), 1, META)){
                fwrite(&MetaBuffer, sizeof(MetaBuffer), 1, Main_Memory);
            }

            while(fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS)){
                fwrite(&BlocBuffer, sizeof(BlocBuffer), 1, Main_Memory);
            }
            break;
    }
}

//initialize the properties of the memory
void initializeDisk(int* NumberBloc, int* FB, int* Org, int* inter) {
    do {
        printf("Enter the Bloc size factor (> 0): ");
        scanf("%d", FB);//scan the bloc size factor
    } while (*FB <= 0);

    do {
        printf("Enter the total number of Blocs (> 0): ");
        scanf("%d", NumberBloc);
    } while (*NumberBloc <= 0);

    do {
        printf("Choose data Organization (1 = Chained, 0 = Contiguous): ");
        scanf("%d", Org);
    } while (*Org != 0 && *Org != 1);

    do {
        printf("Choose internal Orgnization (1 = Sorted, 0 = Unsorted): ");
        scanf("%d", inter);
    } while (*inter != 0 && *inter != 1);

}


//creation of the memory as the file ms, head and meta in caps to differentiate tthem from files, variable or types 
void CreatMS(FILE *Main_Memory, int FB, int NumberBloc, int NumberFile, int Org){
    FILE *MS;
    MS =fopen("memoryS.txt", "wt+");

    FILE *HEAD;
    HEAD = fopen("HEAD.txt", "wt+");

    FILE* META;
    META = fopen("MetaData.txt", "wt+");

    // Allocate memory for the head structure
    MsHead head ;

    head.alloca = (int*)malloc(NumberBloc * sizeof(int));
    if(!head.alloca){
        printf("Memory allocation failed for allocation table\n");
        free(head.alloca);
        exit(1);
    }

    for(int i=0;i<=NumberBloc;i++){
        head.alloca[i] = 0;
    }
        
    head.numberoffiles = 0;

    fwrite(&head, sizeof(head), 1, HEAD);

    //MetaData allocation for file informations 
    for(int i =0;i<NumberFile;i++){
        Meta meta ;
        strcpy(meta.FileName, "\0");
        meta.FileId = i + 1;
        meta.filesizeBloc = 0;
        meta.filesizeRecord = 0;
        meta.firstBlocaddress = -1;
        meta.Global = Org;
        meta.Intern = -1;
        
        fwrite(&meta, sizeof(meta), 1, META);
    }

    //allocation of the blocs for the memory
     Bloc* Blocs = (Bloc*)malloc(NumberBloc * sizeof(Bloc));
    if (!Blocs) {
        printf("Memory allocation failed for Blocs\n");
        free(Blocs);
        exit(1);
    }

    // Initialize each Bloc
    
    for (int i = 0; i < NumberBloc; i++) {
        Blocs[i].id = i;   // Assign Bloc IDs
        if(Org = 1){
            Blocs[i].next = -1;
        }else{
            Blocs[i].next = i+1; 
        }
        

        // Allocate memory for Data in the Bloc
        Blocs[i].Data = (Record*)malloc(FB * sizeof(Record));
        if (!Blocs[i].Data) {
            printf("Memory allocation failed for Data in Bloc %d\n", i + 1);
            for (int j = 0; j < i; j++) { // Free previous Blocs
                free(Blocs[j].Data);
            }
            free(Blocs);
            exit(1);
        }

        // Initialize Data in the Bloc
        for (int j = 0; j < FB; j++) {//
            Blocs[i].Data[j].id = j ; // Assign entry IDs
            Blocs[i].Data[j].data = '\0';    // No data initially
        }

        fwrite(&Blocs, sizeof(Blocs), 1, MS);//write the blocs in the file
    }

    saveMs(MS, HEAD, META, Main_Memory, NumberFile, 4);

    fclose(MS);
    fclose(HEAD);
    fclose(META);


}


// freeing the space in the memory
void freeMS(FILE *Main_Memory, FILE *MS, FILE *HEAD, FILE *META, int Org, int NumberFile){

    rewind(MS);
    rewind(HEAD);
    rewind(META);

    Bloc BlocBuffer;
    Meta MetaBuffer;
    MsHead HeadBuffer;
    int NB = 0;

    //freeing the ms
    while(fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS)){
        
        BlocBuffer.Data = '\0'; // every data eraysed
        if(Org == 1){
            BlocBuffer.next = -1; // if the org is chained no more blocs are chained every bloc becomes independant
        }
        fseek(MS, -sizeof(BlocBuffer), SEEK_CUR); 
        fwrite(&BlocBuffer, sizeof(BlocBuffer), 1, MS);

        NB ++;
    }


    //freeing the head
    fread(&HeadBuffer, sizeof(HeadBuffer), 1, HEAD);
    HeadBuffer.alloca = (int*)malloc(NB * sizeof(int));
    for(int i =0;i<NB;i++){
        HeadBuffer.alloca[i]=0;
    }
    HeadBuffer.numberoffiles = 0;
    seek(HEAD, -sizeof(HeadBuffer), SEEK_CUR);
    fwrite(&HeadBuffer, sizeof(HeadBuffer), 1, HEAD);


    //freeing the Meta
    while(fread(&MetaBuffer, sizeof(MetaBuffer), 1, META)){
        
        strcpy(MetaBuffer.FileName, "\0");
        MetaBuffer.FileId = -1;
        MetaBuffer.filesizeBloc = 0;
        MetaBuffer.filesizeRecord = 0;
        MetaBuffer.firstBlocaddress = -1;
        MetaBuffer.Global = Org;
        MetaBuffer.Intern = -1;

        fseek(META, -sizeof(MetaBuffer), SEEK_CUR);
        fwrite(&MetaBuffer, sizeof(MetaBuffer), 1, META);
    }

    saveMs(MS, HEAD, META, Main_Memory, NumberFile, 4);
}

//returns an array of the index of the free blocs in the memory
int* numFreeBlocs(FILE *HEAD, int* lenght){

    rewind(HEAD);

    int j =0;
    MsHead buffer;

    fread(&buffer, sizeof(buffer), 1, HEAD);
    for(int i=0;i<(sizeof(buffer.alloca)/sizeof(int));i++){
        if(buffer.alloca[i] == 0){
            j++;
        }
    }

    int* freeblocs = (int*)malloc(j * sizeof(int));
    int k = 0;
    for(int i=0;i<(sizeof(buffer.alloca)/sizeof(int));i++){
        if(buffer.alloca[i] == 0){
            freeblocs[i] = i;
        }
    }
    *lenght = j;
    return freeblocs;
}

//creaion of the file and saving it to the memory MS
void creatFile(FILE *Main_Memory, FILE *MS, FILE*HEAD, FILE *META, DataFile File, int NumBlocsFile,
         int NumRecordsFile, int org, int Inter, int FB, int NumberFile){

    FILE* NEWFILE;
    NEWFILE = fopen(File.name, "wt+");

    rewind(META);

    Bloc BlocBuffer;
    Meta MetaBuffer;
    MsHead HeadBuffer;

    bool alloc = allocateBlocs(Main_Memory,MS, HEAD, META, File, NumBlocsFile, NumRecordsFile, org, Inter, FB, NumberFile);
    if(!alloc){
        return;
    }

    rewind(MS);
    
    fseek(META, File.id * sizeof(MetaBuffer), SEEK_SET);
    fread(&MetaBuffer, sizeof(MetaBuffer), 1, META);

    int nrf;

    if(org == 0){
        fseek(MS, MetaBuffer.firstBlocaddress * sizeof(BlocBuffer), SEEK_SET);
        fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS);
        for(int i =0;i<nrf;i++){
            BlocBuffer.Data[i].data = File.data[i];
            if(i  == FB || i == nrf){
                fseek(MS, -sizeof(BlocBuffer), SEEK_CUR);
                fwrite(&BlocBuffer, sizeof(BlocBuffer), 1, MS);

                fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS);

                i = 0;
                nrf -= FB;
            }
        } 
    }else{
        fseek(MS, MetaBuffer.firstBlocaddress * sizeof(BlocBuffer), SEEK_SET);
        fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS);
        for(int i =0;i<nrf;i++){
            BlocBuffer.Data[i].data = File.data[i];
            if(i  == FB || i == nrf){
                int next = BlocBuffer.next;
                fseek(MS, -sizeof(BlocBuffer), SEEK_CUR);
                fwrite(&BlocBuffer, sizeof(BlocBuffer), 1, MS);

                rewind(MS);
                fseek(MS, next * sizeof(BlocBuffer), SEEK_SET);
                fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS);

                i = 0;
                nrf -= FB;
            }
        }
    }

    saveMs(MS, HEAD, META, Main_Memory, NumberFile, 4);
}

//renaming a file
void renameFile(FILE *Main_Memory, FILE* MS, FILE *HEAD, FILE *META, DataFile File, char NewName[50], int NumberFile){
    rewind(META);

    Meta MetaBuffer;

    while(MetaBuffer.FileId != File.id){
        fread(&MetaBuffer, sizeof(MetaBuffer), 1, META);
    }

    strcpy(MetaBuffer.FileName, NewName);

    fseek(META, -sizeof(MetaBuffer), SEEK_CUR);
    fwrite(&MetaBuffer, sizeof(MetaBuffer), 1, META);

    saveMs(MS, HEAD, META, Main_Memory, NumberFile, 4);
}



//allocation or better said reservation of the blocs that a file needs
bool allocateBlocs(FILE *Main_Memory, FILE *MS, FILE*HEAD, FILE *META, DataFile File, int NumBlocsFile, 
        int NumRecordsFile, int org, int Inter, int FB, int NumberFile){

    rewind(MS);
    rewind(HEAD);
    rewind(META);

    Bloc BlocBuffer;
    Meta MetaBuffer;
    MsHead HeadBuffer;

    fread(&HeadBuffer, sizeof(HeadBuffer), 1, HEAD);
        
    int i = 0,lenght, start, blocs = 0;
    int* j = numFreeBlocs(HEAD, &lenght);
    bool found = false;

    if(org == 0){
        while(!found && i != lenght){
            for(int k =0;k<NumBlocsFile-1;k++){
                if(j[i+k+1] == j[i+k]+1){
                    blocs ++;
                }
            }
            if(blocs == NumBlocsFile){
                start = i;
                found = true;
            }else{
                blocs = 0;
                i++;
            }
            
        
        }

        if(!found){
            printf("not enaugh space");
            return false;
        }

        if(NumRecordsFile <= FB){
            Meta meta;

            strcpy(meta.FileName, File.name);
            meta.FileId = File.id;
            meta.filesizeBloc = NumBlocsFile;
            meta.filesizeRecord = NumRecordsFile;
            meta.firstBlocaddress = start;
            meta.Global = org;
            meta.Intern = Inter;

            fseek(META, -1, SEEK_END);
            fwrite(&meta, sizeof(meta), 1, META);

            rewind(HEAD);
            i = meta.firstBlocaddress;
            while(i != (meta.firstBlocaddress + meta.filesizeBloc)){
                HeadBuffer.alloca[i] = 1;
                i++;
            }
            fwrite(&HeadBuffer, sizeof(HeadBuffer), 1, HEAD);

            fseek(MS, meta.firstBlocaddress * sizeof(BlocBuffer), SEEK_SET);

            for(i =0;i<NumBlocsFile-1;i++){
                fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS);
                BlocBuffer.next = BlocBuffer.id + 1;
                fseek(MS, -sizeof(BlocBuffer), SEEK_CUR);
                fwrite(&BlocBuffer, sizeof(BlocBuffer), 1, MS);
            }
            
        }else{
            if(j[start + NumBlocsFile + 1] == j[start + NumBlocsFile ]+1){
                Meta meta;

                strcpy(meta.FileName, File.name);
                meta.FileId = File.id;
                meta.filesizeBloc = NumBlocsFile + 1;
                meta.filesizeRecord = NumRecordsFile;
                meta.firstBlocaddress = start;
                meta.Global = org;
                meta.Intern = Inter;

                fseek(META, -1, SEEK_END);
                fwrite(&meta, sizeof(meta), 1, META);

                rewind(HEAD);
                i = meta.firstBlocaddress;
                while(i != (meta.firstBlocaddress + meta.filesizeBloc)){
                    HeadBuffer.alloca[i] = 1;
                    i++;
                }

                fwrite(&HeadBuffer, sizeof(HeadBuffer), 1, HEAD);

                for(i =0;i<NumBlocsFile-1;i++){
                    fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS);
                    BlocBuffer.next = BlocBuffer.id + 1;
                    fseek(MS, -sizeof(BlocBuffer), SEEK_CUR);
                    fwrite(&BlocBuffer, sizeof(BlocBuffer), 1, MS);
                }

            }else{
                printf("not enaugh space");
                return false;
            }
        }
        
        
    }else{
        if(lenght >= NumBlocsFile){
            if(NumRecordsFile <= FB){
                Meta meta;

                strcpy(meta.FileName, File.name);
                meta.FileId = File.id;
                meta.filesizeBloc = NumBlocsFile;
                meta.filesizeRecord = NumRecordsFile;
                meta.firstBlocaddress = j[0];
                meta.Global = org;
                meta.Intern = Inter;

                fseek(META, -1, SEEK_END);
                fwrite(&meta, sizeof(meta), 1, META);

                rewind(HEAD);
                i = meta.firstBlocaddress;
                while(i != (meta.firstBlocaddress + meta.filesizeBloc)){
                    HeadBuffer.alloca[i] = 1;
                    i++;
                }
                fwrite(&HeadBuffer, sizeof(HeadBuffer), 1, HEAD);
                i =0;
                while(i != NumBlocsFile){
                    fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS);
                    if(BlocBuffer.id == j[i]){
                        BlocBuffer.next = j[i+1];
                        fseek(MS, -sizeof(BlocBuffer), SEEK_CUR);
                        fwrite(&BlocBuffer, sizeof(BlocBuffer), 1, MS);
                        i++;
                    }
                }
            
            }else{
                if(j[start + NumBlocsFile + 1] == j[start + NumBlocsFile ]+1){
                    Meta meta;

                    strcpy(meta.FileName, File.name);
                    meta.FileId = File.id;
                    meta.filesizeBloc = NumBlocsFile + 1;
                    meta.filesizeRecord = NumRecordsFile;
                    meta.firstBlocaddress = j[0];
                    meta.Global = org;
                    meta.Intern = Inter;

                    fseek(META, -1, SEEK_END);
                    fwrite(&meta, sizeof(meta), 1, META);

                    rewind(HEAD);
                    i = meta.firstBlocaddress;
                    while(i != (meta.firstBlocaddress + meta.filesizeBloc)){
                        HeadBuffer.alloca[i] = 1;
                        i++;
                    }
                    fwrite(&HeadBuffer, sizeof(HeadBuffer), 1, HEAD);
                    i =0;
                    while(i != NumBlocsFile){
                        fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS);
                        if(BlocBuffer.id == j[i]){
                            BlocBuffer.next = j[i+1];
                            fseek(MS, -sizeof(BlocBuffer), SEEK_CUR);
                            fwrite(&BlocBuffer, sizeof(BlocBuffer), 1, MS);
                            i++;
                        }
                    }

                }else{
                    printf("not enaugh space");
                    return false;
                }
            }
        }else{
            printf("not enaugh space");
            return false;
        }
    }

    saveMs(MS, HEAD, META, Main_Memory, NumberFile, 4);

    return true;
}


// free up a space used by a file 
void freeBlocs(FILE *Main_Memory, FILE *MS, FILE*HEAD, FILE *META, int org, int FB, int FileId, int NumberFile){
    rewind(MS);
    rewind(HEAD);
    rewind(META);

    Bloc BlocBuffer;
    Meta MetaBuffer;
    MsHead HeadBuffer;

    int NumRecordsFile;
    int NumBlocsFile;
    int firstBlocaddress;

    if(org == 0){
        fseek(&MetaBuffer, FileId * sizeof(MetaBuffer), SEEK_SET);
        fread(&MetaBuffer, sizeof(MetaBuffer), 1, META);

        strcpy(MetaBuffer.FileName, "\0");
        MetaBuffer.FileId = -1;

        NumBlocsFile = MetaBuffer.filesizeBloc;
        MetaBuffer.filesizeBloc = 0;
        
        NumRecordsFile = MetaBuffer.filesizeRecord;
        MetaBuffer.filesizeRecord = 0;

        firstBlocaddress = MetaBuffer.firstBlocaddress;
        MetaBuffer.firstBlocaddress = -1;

        MetaBuffer.Global = org;
        MetaBuffer.Intern = -1;
        
        fseek(MS, firstBlocaddress * sizeof(BlocBuffer), SEEK_SET);
        fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS);

        for(int i =0; i<NumRecordsFile; i++){
            BlocBuffer.Data[i].data = '\0';
            if(i == FB || i == NumRecordsFile){
                BlocBuffer.next = -1;
                fseek(MS, -sizeof(BlocBuffer), SEEK_CUR);
                fwrite(&BlocBuffer, sizeof(BlocBuffer), 1, MS);

                i = 0;
                NumRecordsFile -= FB;

                fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS);
            }
        }

        fread(&HeadBuffer, sizeof(HeadBuffer), 1, HEAD);

        for(int i = FileId; i<(FileId + NumBlocsFile), i++;){
            HeadBuffer.alloca[i] = 0;
        }

        fseek(HEAD, -sizeof(HeadBuffer), SEEK_CUR);
        fwrite(&HeadBuffer, sizeof(HeadBuffer), 1, HEAD);

    }else{
        fseek(&MetaBuffer, FileId * sizeof(MetaBuffer), SEEK_SET);
        fread(&MetaBuffer, sizeof(MetaBuffer), 1, META);

        strcpy(MetaBuffer.FileName, "\0");
        MetaBuffer.FileId = -1;

        NumBlocsFile = MetaBuffer.filesizeBloc;
        MetaBuffer.filesizeBloc = 0;
        
        NumRecordsFile = MetaBuffer.filesizeRecord;
        MetaBuffer.filesizeRecord = 0;

        firstBlocaddress = MetaBuffer.firstBlocaddress;
        MetaBuffer.firstBlocaddress = -1;

        MetaBuffer.Global = org;
        MetaBuffer.Intern = -1;
        
        fseek(MS, firstBlocaddress * sizeof(BlocBuffer), SEEK_SET);
        fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS);

        fread(&HeadBuffer, sizeof(HeadBuffer), 1, HEAD);
        int current = firstBlocaddress;

        for(int i =0; i<NumRecordsFile; i++){
            BlocBuffer.Data[i].data = '\0';
            if(i == FB || i == NumRecordsFile){
                int next = BlocBuffer.next;
                BlocBuffer.next = -1;

                HeadBuffer.alloca[current] = 0;
                current = next;

                fseek(MS, -sizeof(BlocBuffer), SEEK_CUR);
                fwrite(&BlocBuffer, sizeof(BlocBuffer), 1, MS);

                i = 0;
                NumRecordsFile -= FB;

                rewind(MS);
                fseek(MS, next * sizeof(BlocBuffer), SEEK_SET);
                fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS);
            }
        }

        fseek(HEAD, -sizeof(HeadBuffer), SEEK_CUR);
        fwrite(&HeadBuffer, sizeof(HeadBuffer), 1, HEAD);

    }

    saveMs(MS, HEAD, META, Main_Memory, NumberFile, 4);
}


void printFileContent(FILE *MS, FILE*HEAD, FILE *META, int org, int Inter, int FB, int FileId){
    rewind(MS);
    rewind(META);

    Bloc BlocBuffer;
    Meta MetaBuffer;
    MsHead HeadBuffer;

    fseek(&MetaBuffer, FileId * sizeof(MetaBuffer), SEEK_SET);
    fread(&MetaBuffer, sizeof(MetaBuffer), 1, META);
    
    fseek(MS, MetaBuffer.firstBlocaddress * sizeof(BlocBuffer), SEEK_SET);
    fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS);

    if(org == 0){
        for(int i =0; i<MetaBuffer.filesizeBloc; i++){
            for(int j=0; j<FB; j++){
                printf("%s  ",BlocBuffer.Data[j].data);
            }
            fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS);
        }
    }else{
        for(int i =0; i<MetaBuffer.filesizeBloc; i++){

            for(int j=0; j<FB; j++){
                printf("%s  ",BlocBuffer.Data[j].data);
            }

            int next = BlocBuffer.next;

            rewind(MS);
            fseek(MS, next * sizeof(BlocBuffer), SEEK_SET);
            fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS);
            
        }
    }
}

//save a given file to the disc if there are enough space
void saveFileToDisk(FILE *Main_Memory, FILE *File, FILE *MS, FILE*HEAD, FILE *META, char filename[50], int org, int inter, int FB, int NumBlocsFile, 
        int NumRecordsFile, int NumberFile){
    
    rewind(MS);
    rewind(HEAD);
    rewind(META);

    Bloc BlocBuffer;
    Meta MetaBuffer;
    MsHead HeadBuffer;
    DataFile FileBuffer;

    fread(&FileBuffer.data, sizeof(FileBuffer.data), 1, File);
    strcpy(FileBuffer.name , filename);

    fseek(META, 0, SEEK_END);
    fread(&MetaBuffer, sizeof(MetaBuffer), 1, META);

    FileBuffer.id = MetaBuffer.FileId + 1;

    bool alloc = allocateBlocs(Main_Memory,MS, HEAD, META, FileBuffer, NumBlocsFile, NumRecordsFile, org, inter, FB, NumberFile);
    if(!alloc){
        return;
    }

    rewind(MS);
    rewind(HEAD);
    rewind(META);

    fseek(MS, MetaBuffer.firstBlocaddress * sizeof(BlocBuffer), SEEK_SET);
    fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS);

    int i =0;
    int nrf = NumRecordsFile;

    while(i <= nrf){
        BlocBuffer.Data[i].data = FileBuffer.data[i];
        if(i == FB || i == nrf){
            int next = BlocBuffer.next;
            fseek(MS, -sizeof(BlocBuffer), SEEK_CUR);
            fwrite(&BlocBuffer, sizeof(BlocBuffer), 1, MS);

            rewind(MS);
            fseek(MS, next * sizeof(BlocBuffer), SEEK_SET);
            fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS);

            i = 0;
            nrf -= FB;
        }
    }

    saveMs(MS, HEAD, META, Main_Memory, NumberFile, 4);
    
}

//load a specific file into an array of buffers the size of the file 
Bloc* loadFileFromDisk(FILE *MS, FILE*HEAD, FILE *META, DataFile File, int org, int FB){

    rewind(MS);
    rewind(HEAD);
    rewind(META);

    
    Meta MetaBuffer;
    MsHead HeadBuffer;

    fseek(META, File.id * sizeof(MetaBuffer), SEEK_SET);
    fread(&MetaBuffer, sizeof(MetaBuffer), 1, META); 
    
    Bloc BlocBuffer[MetaBuffer.filesizeBloc];

    int i =0, j =0;
    int nrf = MetaBuffer.filesizeRecord;
    int nbf = MetaBuffer.filesizeBloc;

    fseek(MS, MetaBuffer.firstBlocaddress * sizeof(BlocBuffer), SEEK_SET);
    

    while(i <= nbf){
        while(j <= nrf){
            fread(&BlocBuffer[i].Data[j], sizeof(BlocBuffer[i].Data[j]), 1, MS);
            if(j == FB || j == nrf){
                i ++;
                j =0;
                nrf -= FB;
            }
        }
        
    }

    return BlocBuffer;

}

/*----------------------------------------------------------------------------------------------------*/


/*-------------------------------------- MOZALI PART -------------------------------------------------*/

void insertRecord(Meta* meta, Bloc* Block, int filenumber, char data, int* FB) {
    Bloc buffer = Block[filenumber];  // Create buffer from the Bloc array

    for (int i = 0; i < meta[filenumber].filesizeBloc; i++) {
        for (int j = 0; j < *FB; j++) {
            if (buffer.Data[j].data == '\0') {
                buffer.Data[j].id = j;
                buffer.Data[j].data = data;
                buffer.Data[j].deleted = false;
                Block[filenumber] = buffer;  // Save changes back to array
                printf("Record inserted successfully.\n");
                return;
            }
        }
    }
    printf("No space available to insert the record, we gonna allocate a new bloc for this record \n");
    int newblockindex = meta[filenumber].filesizeBloc;
    meta[filenumber].filesizeBloc = meta[filenumber].filesizeBloc + 1;

    buffer.Data = (Record*)malloc(*FB * sizeof(Record));
    if (!buffer.Data) {
        printf("Memory allocation failed for new block.\n");
        return;
    }

    buffer.id = newblockindex;
    buffer.next = -1;

    buffer.Data[0].id = 0;
    buffer.Data[0].data = data;
    buffer.Data[0].deleted = false;

    for (int j = 1; j < *FB; j++) {
        buffer.Data[j].data = '\0';
        buffer.Data[j].deleted = false;
    }

    Block[filenumber] = buffer;  // Save the new block back to array
    printf("New block allocated and record inserted successfully.\n");
}

void searchRecord(MsHead* head, Meta* meta, Bloc* Block, int filenumber, int recordID, int* FB, int* blockNum, int* recordNum) {
    if (filenumber >= head->numberoffiles) {
        printf("Invalid file number.\n");
        *blockNum = -1;
        *recordNum = -1;
        return;
    }

    Bloc buffer = Block[filenumber];  // Create buffer from the Bloc array

    for (int i = 0; i < meta[filenumber].filesizeBloc; i++) {
        for (int j = 0; j < *FB; j++) {
            Record currentRecord = buffer.Data[j];
            if (currentRecord.id == recordID) {
                if (!currentRecord.deleted) {
                    printf("Record found in block %d, entry %d.\n", i + 1, j + 1);
                    *blockNum = i;
                    *recordNum = j;
                    return;
                } else {
                    printf("Record with ID %d is logically deleted.\n", recordID);
                    *blockNum = -1;
                    *recordNum = -1;
                    return;
                }
            }
        }
    }
    printf("Record with ID %d not found.\n", recordID);
}

void deleteRecord(MsHead* head, Meta* meta, Bloc* Block, int filenumber, int recordID, int* FB) {
    if (filenumber >= head->numberoffiles) {
        printf("Invalid file number.\n");
        return;
    }

    Bloc buffer = Block[filenumber];  // Create buffer from the Bloc array

    for (int i = 0; i < meta[filenumber].filesizeBloc; i++) {
        for (int j = 0; j < *FB; j++) {
            if (buffer.Data[j].id == recordID) {
                if (!buffer.Data[j].deleted) {
                    buffer.Data[j].deleted = true;
                    Block[filenumber] = buffer;  // Save changes back to array
                    printf("Record with ID %d logically deleted.\n", recordID);
                    return;
                } else {
                    printf("Record with ID %d is already logically deleted.\n", recordID);
                    return;
                }
            }
        }
    }
    printf("Record with ID %d not found.\n", recordID);
}
void physicalDeleteRecord(MsHead* head, Meta* meta, Bloc* Block, int filenumber, int recordID, int* FB) {
    if (filenumber >= head->numberoffiles) {
        printf("Invalid file number.\n");
        return;
    }

    int blockNum, recordNum;
    Bloc buffer = Block[filenumber];  // Create buffer from the Bloc array

    // Use searchRecord to find the record
    searchRecord(head, meta, Block, filenumber, recordID, FB, &blockNum, &recordNum);

    if (blockNum != -1 && recordNum != -1) {
        // Shift all records after the deleted one
        for (int j = recordNum; j < *FB - 1; j++) {
            buffer.Data[j] = buffer.Data[j + 1];
        }

        // Clear the last record
        buffer.Data[*FB - 1].data = '\0';
        buffer.Data[*FB - 1].id = -1;
        buffer.Data[*FB - 1].deleted = false;

        // Update the metadata
        meta[filenumber].filesizeRecord--;

        // Save changes back to array
        Block[filenumber] = buffer;

        printf("Record with ID %d physically deleted.\n", recordID);
    } else {
        printf("Record with ID %d not found or already deleted.\n", recordID);
    }
}

/*----------------------------------------------------------------------------------------------------*/





/*-------------------------------------- Mehdi's Part --------------------------------------------------*/

void DeleteFile(char name[50], int numoffile, MsHead* head,  Meta* meta, Bloc* Bloc){

    if (head == NULL || meta == NULL) {        //head ou head->meta ne sont pas allouée donc il ne peut pas y'avoir de fichier

        printf("Error, invalid metadata structure.\n");

        return;

    }
    

    for (int i = 0; i < head->numberoffiles ; i++){

        if (strcmp(meta[i].FileName, name) == 0){

            if(head->numberoffiles == 1){       // Si il y'a qu'un seul élément libérer la mémoire allouée 

                free(meta);

                meta = NULL;      //NULL proposition ta3 chatgpt bech tiviti les pointeur "dangereux"

                head->numberoffiles = 0;

            }else{

                for (int j = i; j < numoffile-1; j++){    //Ecraser le fichier qu'on veut supprimer et decaler les autres fichiers

                    meta[j]=meta[j+1];

                }

                head->numberoffiles--;     //Le nombre de fichier diminue de 1

                printf("The file '%s' has been deleted.", name);

                return ;

            }    
            
        }
        
    
    }

    printf("The file '%s' doesn't exist.", name);      //Si le fichier n'existe pas

    return ;

}




void PopulateFile(int filenumber, int numofrecord, MsHead* head, Meta* meta, Bloc* Bloc, int* FB){

    bool exist = false;   //variable booleene initialiser a 'false' car on admet que le fichier n'existe pas

    if (head == NULL || meta == NULL) {        //head ou head->meta ne sont pas allouée donc il ne peut pas y'avoir de fichier

        printf("Error, invalid metadata structure.\n");

        return;

    }
    
    if (meta->filesizeRecord+numofrecord <= *FB){     //3andna assez d'espace bech nzidou numofrecord
        
        for (int i = 0; i < head->numberoffiles; i++){     //boucle bech n7awes 3la file dyali
        
            if (filenumber == Bloc[i].id){
                
                exist = true;     //le fichier existe

                for (int j = 0; j < numofrecord; j++){
            
                    insertRecord(meta, Bloc, filenumber,rand() % 128,FB);   //insert numofrecord record w comme data random char
                    
                }       
            
            }
        
        }

        if(exist){
        
            meta->filesizeRecord+=numofrecord;   //mise a jour du nombre de record dans le fichier

            printf("%d records successfully populated into file '%d'.\n",numofrecord,filenumber);
        
        }else{

            printf("The file '%d' doesn't exist.",filenumber);

        }
        
    }else{    // ma3andnach assez d'espace bech ndekhlou numofrecord

        printf("Error: Not enough space to add '%d' record(s).\n",numofrecord);
    
    }
    
    
}    




/*----------------------------------------------------------------------------------------------------*/





/*-------------------------------------- STAMBOULI PART ----------------------------------------------*/
// Fonction pour la défragmentation
void defragment_file(FILE *Main_Memory, FILE *MS, FILE *HEAD, FILE *META, DataFile *File, int NumberBlocMax, int NumberFile) {
    // Charger les métadonnées
    Meta meta;
    fseek(META, sizeof(Meta) * File->id, SEEK_SET);
    fread(&meta, sizeof(Meta), 1, META);

    if (meta.filesizeBloc <= 0) {
        printf("Erreur : Le fichier '%s' n'existe pas ou est vide.\n", File->name);
        return;
    }

    // Charger la table d'allocation
    MsHead ms_head;
    fseek(HEAD, 0, SEEK_SET);
    fread(&ms_head, sizeof(MsHead), 1, HEAD);

    int *allocation_table = (int *)malloc(NumberBlocMax * sizeof(int));
    fseek(HEAD, sizeof(MsHead), SEEK_SET);
    fread(allocation_table, sizeof(int), NumberBlocMax, HEAD);

    int current_block = meta.firstBlocaddress;
    int new_start_block = -1;

    // Trouver le premier bloc libre pour la défragmentation
    for (int i = 0; i < NumberBlocMax; i++) {
        if (allocation_table[i] == 0) {
            new_start_block = i;
            break;
        }
    }

    if (new_start_block == -1) {
        printf("Erreur : Pas d'espace libre pour défragmenter '%s'.\n", File->name);
        free(allocation_table);
        return;
    }

    int next_free_block = new_start_block;

    // Réorganisation des blocs
    for (int i = 0; i < meta.filesizeBloc; i++) {
        Bloc bloc;
        fseek(MS, current_block * sizeof(Bloc), SEEK_SET);
        fread(&bloc, sizeof(Bloc), 1, MS);

        // Écrire le bloc au nouvel emplacement
        bloc.id = next_free_block;
        fseek(MS, next_free_block * sizeof(Bloc), SEEK_SET);
        fwrite(&bloc, sizeof(Bloc), 1, MS);

        // Mettre à jour la table d'allocation
        allocation_table[next_free_block] = 1;
        allocation_table[current_block] = 0;

        if (meta.Global && bloc.next != -1) {
            current_block = bloc.next;
        } else {
            current_block++;
        }

        next_free_block++;
    }

    // Mise à jour des métadonnées
    meta.firstBlocaddress = new_start_block;
    fseek(META, sizeof(meta) * File->id, SEEK_SET);
    fwrite(&meta, sizeof(meta), 1, META);

    // Mise à jour de la table d'allocation
    fseek(HEAD, sizeof(MsHead), SEEK_SET);
    fwrite(allocation_table, sizeof(int), NumberBlocMax, HEAD);

    printf("Fichier '%s' défragmenté avec succès.\n", File->name);

    free(allocation_table);

    saveMs(MS, HEAD, META, Main_Memory, NumberFile, 4);
}


/*--------------------------------------------------------------------------------------------------*/



//*************************************************************  Main function**********************************************


void main(){
    // VARIABLES----------------

        //----MAIN MEMORY FILE----
        FILE* Main_Memory;
        Main_Memory = fopen("Main_Memory.txt", "wt+");

        // BUFFERS------
        Bloc BlocBuffer;
        Meta MetaBuffer;
        MsHead HeadBuffer;
        DataFile FileBuffer;

        //VARIABLES--------
        int NumberBloc;
        int FB;
        int Org;
        int inter;
        int NumberFile;
        int lenghtFreeBlocArray; 
        int NumBlocsFile;
        int NumRecordsFile;
        int* BlocsId;
        char data;
        int recordID;
        int i = 0;
        int TaskChoice;

        //FILES-------(MS ,HEAD and META are created with creatMS() use it to creat them)---------
        DataFile File;

        do{
            printf("how many files do you want to store at maximum  :  ");
            scanf("%d", &NumberFile);
            while (getchar() != '\n'); // Clear the input buffer
        }while(NumberFile <= 0);

        DataFile Files[NumberFile];
    //---------------------------

    printf("\n \n \n ---------- ENTERING TERMINAL ----------------");
    while (true)
    {
        printf("\n \nPlease select an operation   :\n");
        printf("1 - Initialize system\n");
        printf("2 - Create a new file\n");
        printf("3 - Automatically fill a file\n");
        printf("4 - Rename a file\n");
        printf("5 - Add a record\n");
        printf("6 - Search for record by ID\n");
        printf("7 - Logically delete a record\n");
        printf("8 - Physically delete a record\n");
        printf("9 - Defragmentation\n");
        printf("10 - Print file content\n");
        printf("11 - Save file to disk\n");
        printf("12 - Delete a file\n");
        printf("13 - Clear memory structure\n");
        printf("14 - Exit\n \n \n ");

        scanf("%d", &TaskChoice);


        switch (TaskChoice){
            
        case 1:
            printf("Initializing system...\n");
            break;
        case 2:
            printf("Creating a new file...\n");
            break;
        case 3:
            printf("Please enter the ID of the file you want to automatically fill :\n");
        case 4:
            printf("Renaming a file...\n");
            break;
        case 5:
            printf("Adding a record...\n");
            break;
        case 6:
            printf("Searching for record by ID...\n");
            break;
        case 7:
            printf("Logically deleting a record...\n");
            break;
        case 8:
            printf("Physically deleting a record...\n");
            break;
        case 9:
            printf("Defragmenting...\n");
            break;
        case 10:
            printf("Printing file content...\n");
            break;
        case 11:
            printf("Saving file to disk...\n");
            break;
        case 12:
            printf("Please enter the name of the file you want to delete :\n");
            break;
        case 13:
            printf("Clearing memory structure...\n");
            break;
        case 14:
            printf("Exiting the program. Goodbye!\n");
            exit(0);
        default:
            printf("Invalid choice. Please select a valid operation.\n");
            break;
        }
    }
}
