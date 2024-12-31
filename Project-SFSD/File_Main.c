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
void CreatMS(int FB, int NumberBloc, int NumberFile, int Org){
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

    fclose(MS);
    fclose(HEAD);
    fclose(META);
}


// freeing the space in the memory
void freeMS(FILE *MS, FILE *HEAD, FILE *META, int Org){

    rewind(MS);
    rewind(HEAD);
    rewind(META);

    Bloc BlocBuffer;
    Meta MetaBuffer;
    MsHead HeadBuffer;
    int NB = 0;

    //freeing the ms
    while(fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS)){
        fseek(MS, sizeof(BlocBuffer), SEEK_CUR); 
        BlocBuffer.Data = '\0'; // every data eraysed
        if(Org == 1){
            BlocBuffer.next = -1; // if the org is chained no more blocs are chained every bloc becomes independant
        }
        fwrite(&BlocBuffer, sizeof(BlocBuffer), 1, MS);

        NB ++;
    }


    //freeing the head
    fseek(HEAD, sizeof(HeadBuffer), SEEK_CUR);
    HeadBuffer.alloca = (int*)malloc(NB * sizeof(int));
    for(int i =0;i<NB;i++){
        HeadBuffer.alloca[i]=0;
    }
    HeadBuffer.numberoffiles = 0;
    fwrite(&HeadBuffer, sizeof(HeadBuffer), 1, HEAD);


    //freeing the Meta
    while(fread(&MetaBuffer, sizeof(MetaBuffer), 1, META)){
        fseek(META, sizeof(MetaBuffer), SEEK_CUR);
        strcpy(MetaBuffer.FileName, "\0");
        MetaBuffer.FileId = -1;
        MetaBuffer.filesizeBloc = 0;
        MetaBuffer.filesizeRecord = 0;
        MetaBuffer.firstBlocaddress = -1;
        MetaBuffer.Global = Org;
        MetaBuffer.Intern = -1;

        fwrite(&MetaBuffer, sizeof(MetaBuffer), 1, META);
    }
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
void creatFile(FILE *MS, FILE*HEAD, FILE *META, DataFile File, int NumBlocsFile,
         int NumRecordsFile, int org, int Inter, int FB, int Numberbloc){

    FILE* NEWFILE;
    NEWFILE = fopen(File.name, "wt+");

    rewind(META);

    Bloc BlocBuffer;
    Meta MetaBuffer;
    MsHead HeadBuffer;

    bool alloc = allocateBlocs(MS, HEAD, META, File, NumBlocsFile, NumRecordsFile, org, Inter, FB, Numberbloc);
    if(!alloc){
        return;
    }

    rewind(MS);
    
    fseek(META, File.id * sizeof(MetaBuffer), SEEK_SET);
    fread(&MetaBuffer, sizeof(MetaBuffer), 1, META);

    if(org == 0){
        fseek(MS, MetaBuffer.firstBlocaddress * sizeof(BlocBuffer), SEEK_SET);
        fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS);
        for(int i =0;i<NumRecordsFile;i++){
            BlocBuffer.Data[i].data = File.data[i];
            if((i+1)  == FB){
                fseek(MS, -sizeof(BlocBuffer), SEEK_CUR);
                fwrite(&BlocBuffer, sizeof(BlocBuffer), 1, MS);

                for(int k =0; k<FB; k++){
                    fwrite(&BlocBuffer.Data[i], sizeof(BlocBuffer.Data[i]), 1, NEWFILE);
                }

                fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS);
            }
        } 
    }else{
        fseek(MS, MetaBuffer.firstBlocaddress * sizeof(BlocBuffer), SEEK_SET);
        fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS);
        for(int i =0;i<NumRecordsFile;i++){
            BlocBuffer.Data[i].data = File.data[i];
            if((i+1)  == FB){
                int next = BlocBuffer.next;
                fseek(MS, -sizeof(BlocBuffer), SEEK_CUR);
                fwrite(&BlocBuffer, sizeof(BlocBuffer), 1, MS);

                for(int k =0; k<FB; k++){
                    fwrite(&BlocBuffer.Data[i], sizeof(BlocBuffer.Data[i]), 1, NEWFILE);
                }
                
                rewind(MS);
                fseek(MS, next * sizeof(BlocBuffer), SEEK_SET);
                fread(&BlocBuffer, sizeof(BlocBuffer), 1, MS);
            }
        }
    }

}

//renaming a file
void renameFile(FILE *META, DataFile File, char NewName[50]){
    rewind(META);

    Meta MetaBuffer;

    while(MetaBuffer.FileId != File.id){
        fread(&MetaBuffer, sizeof(MetaBuffer), 1, META);
    }

    strcpy(MetaBuffer.FileName, NewName);

    fseek(META, -sizeof(MetaBuffer), SEEK_CUR);
    fwrite(&MetaBuffer, sizeof(MetaBuffer), 1, META);
}

/*-------------------------------------- FAHD PART ---------------------------------------------------*/

//allocation or better said reservation of the blocs that a file needs
bool allocateBlocs(FILE *MS, FILE*HEAD, FILE *META, DataFile File, int NumBlocsFile, 
        int NumRecordsFile, int org, int Inter, int FB, int Numberbloc){

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

    return true;
}

void freeBlocs(FILE *MS, FILE*HEAD, FILE *META, int org, int Inter, int FB, int* BlocsId){
    
}


/*----------------------------------------------------------------------------------------------------*/


/*-------------------------------------- MOZALI PART -------------------------------------------------*/

void insertRecord(Meta* meta, Bloc* Bloc, int filenumber, char data, int* FB) {//insert record in the file 

    for (int i = 0; i < meta[filenumber].filesizeBloc; i++) {
        for (int j = 0; j < *FB; j++) {
            if (Bloc[i].Data[j].data == '\0') {
                Bloc[i].Data[j].id = j;
                Bloc[i].Data[j].data = data;
                Bloc[i].Data[j].deleted = false;
                printf("Record inserted successfully.\n");
                return;
            }
        }
    }
   printf("No space available to insert the record.\n");
}



// Function to search for a record by ID
void searchRecord(MsHead* head, Meta* meta, Bloc* Bloc, int filenumber, int recordID, int* FB) {//search for a record by ID in the file
    if (filenumber >= head->numberoffiles) {//if the file number is invalid
        printf("Invalid file number.\n");
        return;
    }

    for (int i = 0; i < meta[filenumber].filesizeBloc; i++) {//get the blocs of the file we want to search in
        struct Bloc currentBloc = Bloc[i];//get the current bloc
        for (int j = 0; j < *FB; j++) {
            Record currentRecord = currentBloc.Data[j];//get the current record in the bloc
            if (currentRecord.id == recordID) {//if the record id is the same as the one we are searching for
                if (!currentRecord.deleted) {//if the record is not logically deleted
                    printf("Record found in block %d, entry %d.\n", i + 1, j + 1);
                    return;
                } else {
                    printf("Record with ID %d is logically deleted.\n", recordID);//if the record is logically deleted
                    return;
                }
            }
        }
    }
    printf("Record with ID %d not found.\n", recordID);
}
// Function to logically delete a record
void deleteRecord(MsHead* head, Meta* meta, Bloc* Bloc, int filenumber, int recordID, int* FB) {
    if (filenumber >= head->numberoffiles) {//if the file number is invalid
        printf("Invalid file number.\n");
        return;
    }

    for (int i = 0; i < meta[filenumber].filesizeBloc; i++) {//get the blocs of the file we want to search in
        for (int j = 0; j < *FB; j++) {//search for the record in the bloc
            if (Bloc[i].Data[j].id == recordID) {//if the record id is the same as the one we are searching for
                if (!Bloc[i].Data[j].deleted) {
                    Bloc[i].Data[j].deleted = true;//logically delete the record
                    printf("Record with ID %d logically deleted.\n", recordID);//
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



/*--------------------------------------------------------------------------------------------------*/



//*************************************************************  Main function**********************************************


void main(){
    // VARIABLES----------------

        // BUFFERS------
        Bloc BlocBuffer;
        Meta MetaBuffer;
        MsHead HeadBuffer;

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