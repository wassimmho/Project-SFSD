#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "../include/raylib.h"

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

typedef struct Ms { // Main structure
    struct Bloc* Bloc;   // Dynamic array of Blocs
} Ms;

typedef struct Meta { // Meta data structure fih les informations li 3andna 3la les fichiers
    char name[50];           // File name
    int filesizeBloc;       // File size in Blocs
    int filesizeRecord;        // File size in Data
    int firstBlocaddress;   // Address of the first Bloc
    bool Global;             // Organization mode (1 = chained, 0 = contiguous)
    bool Intern;             // Sorted Data (1 = sorted, 0 = unsorted)
} Meta;

typedef struct MsHead { // Head block structure
    int* alloca;             // Allocation array
    Meta* meta;              // Meta data array
    int numberoffiles;       // Number of files
    Ms *body;              // Ms body fih les blocs

} MsHead;

typedef struct File{
    char name[50];
    int id;
};


// ************************Create scondary memory body and head ***********************************************
MsHead* createMS(int* allocat, int n, int FB,int numoffile) {//n is the number of blocs and FB is the number of records in each bloc

    FILE *MS;
    MS =fopen("memoryS.txt", "wt+");

    FILE *HEAD;
    HEAD = fopen("HEAD.txt", "wt+");

    // Allocate memory for the head structure
    MsHead* head = (MsHead*)malloc(sizeof(MsHead)); // Allocate memory for the head structure
    if (!head) {
        printf("Memory allocation failed for head\n");
        exit(1);
    }

    // Assign the allocation table and its size
    head->alloca = allocat;
    head->numberoffiles=0;

    // Allocate memory for the Ms structure (body)
    Ms* thebody = (Ms*)malloc(sizeof(Ms));
    if (!thebody) {
        printf("Memory allocation failed for body\n");
        free(head);
        exit(1);
    }

    // Allocate memory for the Blocs array
    Bloc* Blocs = (Bloc*)malloc(n * sizeof(Bloc));
    if (!Blocs) {
        printf("Memory allocation failed for Blocs\n");
        free(thebody);
        free(head);
        exit(1);
    }
      // Allocate memory for the meta array
    head->meta = (Meta*)malloc( n * sizeof(Meta)); // hadi rani zdtha wassim chofha 
    if (!head->meta) {
        printf("Memory allocation failed for meta\n");
        free(Blocs);
        free(thebody);
        free(head);
        exit(1);
    }


    // Initialize each Bloc
    for (int i = 0; i < n; i++) {
        Blocs[i].id = i ;   // Assign Bloc IDs
        Blocs[i].next = -1;    // No next Bloc by default

        // Allocate memory for Data in the Bloc
        Blocs[i].Data = (Record*)malloc(FB * sizeof(Record));
        if (!Blocs[i].Data) {
            printf("Memory allocation failed for Data in Bloc %d\n", i + 1);
            for (int j = 0; j < i; j++) { // Free previous Blocs
                free(Blocs[j].Data);
            }
            free(Blocs);
            free(thebody);
            free(head);
            exit(1);
        }

        // Initialize Data in the Bloc
        for (int j = 0; j < FB; j++) {//
            Blocs[i].Data[j].id = j ; // Assign entry IDs
            Blocs[i].Data[j].data = '\0';    // No data initially
        }

        fwrite(&Blocs, sizeof(Blocs), 1, MS);//write the blocs in the file
        
    }

    // Assign Blocs to the body
    thebody->Bloc = Blocs;//assign the blocs to the body

    // Link the body to the head
    head->body = thebody;//assign the body to the head

    printf("MS structure created with %d Blocs, each containing %d Data.\n", n, FB);
    return head;

}

// initialise sorting modes allocation table ************************************************************** 
void initializeDisk(int* NB, int* FB, int* Orga, int* inter, int** vector) {//NB is the number of blocs, FB is the number of records in each bloc, Orga is the organization mode, inter is the sorting mode and vector is the allocation table
    
    do {
        printf("Enter the Bloc size factor (> 0): ");
        scanf("%d", FB);//scan the bloc size factor
    } while (*FB <= 0);

    do {
        printf("Enter the total number of Blocs (> 0): ");
        scanf("%d", NB);
    } while (*NB <= 0);

    do {
        printf("Choose data organization (1 = Chained, 0 = Contiguous): ");
        scanf("%d", Orga);
    } while (*Orga != 0 && *Orga != 1);

    do {
        printf("Choose internal organization (1 = Sorted, 0 = Unsorted): ");
        scanf("%d", inter);
    } while (*inter != 0 && *inter != 1);

    *vector = (int*)malloc(*NB * sizeof(int)); // Allocate memory for the allocation table
    if (!*vector) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    memset(*vector, 0, *NB * sizeof(int));//initialize all element in the allocation table with 0
    printf("Allocation table initialized with %d cells, all set to 0.\n", *NB);
}

//*********************************** 
void freeMS(MsHead* head,int n) {
    if (head == NULL) return;

    // Free allocation table
    free(head->alloca);

    // Free metadata
    free(head->meta);

    // Free Blocs
    if (head->body != NULL) {
        if (head->body->Bloc != NULL) {
            for (int i = 0; i < n; i++) {
                free(head->body->Bloc[i].Data);
            }
            free(head->body->Bloc);
        }
        free(head->body);
    }

    // Free head
    free(head);
}

int* numFreeBlocs(int* allocationT, int* NB){

    int j=0  ;
    for (int i = 0; i < *NB; i++)
    {
        if (allocationT[i]==0)
        {
        j++;  
        }
        
    }

    int* freeblocs = (int*)malloc(j * sizeof(int));

    int k = 0;

    for (int i = 0; i < *NB; i++){
        if (allocationT[i]==0){

            freeblocs[k] = i;
            k++;
        }
        
    }

    return freeblocs;
}

// creat file isnt done yet
void createfile(MsHead* head,char name[50],int NOR ,int* GO, int* IO,int* filenumber,int* FB,int* NB){

        int k = numFreeBlocs(head->alloca, NB);
        printf("%d \n", k);//number of free blocs print just to check ida raho yemchi
        head->numberoffiles=head->numberoffiles+1;

        while (getchar() != '\n');// clear buffer tferegh buffer mour masste3melna scanf prs scanf y 5eli mourah \n li yreje3 fget tedi hadak character w te5roj bla mate9ra input donc like nta derte entrer fget te9ra hadik entrer li de5eltha nta mo9bil w tssoti direkte

        printf("would you provide the file's name : ");
        fgets(name, 50, stdin); // scanf mais bach te9ra the hole line with space ( 3efssa te3 buffer)
        name[strcspn(name, "\n")] = '\0';
        puts("");

        strcpy(head->meta[*filenumber].name, name); 
        head->meta[*filenumber].filesizeBloc =(int)ceil((double)NOR / *FB);    // [NOR / FB ]+1
        head->meta[*filenumber].filesizeRecord =NOR;
        head->meta[*filenumber].Global=*GO;
        head->meta[*filenumber].Intern=*IO;

       
}

void Renamefile (MsHead* head,char newname[50], int filenumber, int numoffiles){
        
        printf("please you provide the new file's name (no space use _ instead )   :   ");
        fgets(newname, 50, stdin);
        newname[strcspn(newname, "\n")] = '\0';

        puts("");// tssouti ster

    for (int i = 0; i < numoffiles; i++) {
        if (i != filenumber && strcmp(head->meta[i].name, newname) == 0) {
            printf("Error: A file with the name '%s' already exists.\n", newname);
            return ;  // Duplicate name found
        }
    }


    strcpy(head->meta[filenumber].name, newname); //rename the file 
    printf("File has been successfully renamed to '%s'.\n", newname);
    
    return ;
}




/*-------------------------------------- FAHD PART ---------------------------------------------------*/





/*----------------------------------------------------------------------------------------------------*/








/*-------------------------------------- MOZALI PART -------------------------------------------------*/

void insertRecord(MsHead* head, int filenumber, char data, int* FB) {//insert record in the file 

    for (int i = 0; i < head->meta[filenumber].filesizeBloc; i++) {
        for (int j = 0; j < *FB; j++) {
            if (head->body->Bloc[i].Data[j].data == '\0') {
                head->body->Bloc[i].Data[j].id = j;
                head->body->Bloc[i].Data[j].data = data;
                head->body->Bloc[i].Data[j].deleted = false;
                printf("Record inserted successfully.\n");
                return;
            }
        }
    }
   printf("No space available to insert the record, we gonna allocate a new bloc for this record \n");
    int newblockindex = head->meta[filenumber].filesizeBloc;//index ta3 nouveau bloc
    head->meta[filenumber].filesizeBloc=head->meta[filenumber].filesizeBloc+1;//incrementation du nombre de blocs

   
    head->body->Bloc[newblockindex].Data = (Record*)malloc(*FB * sizeof(Record)); //allocation de la memoire pour le nouveau bloc
    if (!head->body->Bloc[newblockindex].Data) {
        printf("Memory allocation failed for new block.\n");
        return;
    }

    
    head->body->Bloc[newblockindex].id = newblockindex; 
    head->body->Bloc[newblockindex -1].next = newblockindex;//le dernier bloc pointe sur le nouveau bloc
    head->body->Bloc[newblockindex].next = -1;// le dernier bloc m3ndoch 3la chkon ypointi

    
    head->body->Bloc[newblockindex].Data[0].id = 0;
    head->body->Bloc[newblockindex].Data[0].data = data;
    head->body->Bloc[newblockindex].Data[0].deleted = false;

   
    for (int j = 1; j < *FB; j++) {// initialisation ta3 les records li b9aw f le nouveau bloc
        head->body->Bloc[newblockindex].Data[j].data = '\0';
        head->body->Bloc[newblockindex].Data[j].deleted = false;
    }

    printf("New block allocated and record inserted successfully.\n");
}

         


// Function to search for a record by ID
void searchRecord(MsHead* head, int filenumber, int recordID, int* FB, int* blockNum, int* recordNum) {//search for a record by ID in the file
    if (filenumber >= head->numberoffiles) {//if the file number is invalid
        printf("Invalid file number.\n");
        *blockNum = -1;
        *recordNum = -1;
        return;
    }

    for (int i = 0; i < head->meta[filenumber].filesizeBloc; i++) {//get the blocs of the file we want to search in
        Bloc currentBloc = head->body->Bloc[i];//get the current bloc
        for (int j = 0; j < *FB; j++) {
            Record currentRecord = currentBloc.Data[j];//get the current record in the bloc
            if (currentRecord.id == recordID) {//if the record id is the same as the one we are searching for
                if (!currentRecord.deleted) {//if the record is not logically deleted
                    printf("Record found in block %d, entry %d.\n", i + 1, j + 1);
                    *blockNum = i;
                    *recordNum = j;
                    return;
                } else {
                    printf("Record with ID %d is logically deleted.\n", recordID);//if the record is logically deleted
                    *blockNum = -1;
                    *recordNum = -1;
                    return;
                }
            }
        }
    }
    printf("Record with ID %d not found.\n", recordID);
    *blockNum = -1;
    *recordNum = -1;
} 


void deleteRecord(MsHead* head, int filenumber, int recordID, int* FB) {// Function to logically delete a record
    if (filenumber >= head->numberoffiles) {//ida le file makach 
        printf("Invalid file number.\n");
        return;
    }

    int blockNum, recordNum;//variables to store the block number and the record number
    searchRecord(head, filenumber, recordID, FB, &blockNum, &recordNum);//search for the record li rana habin nfasoh
    
    if (blockNum != -1 && recordNum != -1) {//if the record exists
        if (!head->body->Bloc[blockNum].Data[recordNum].deleted) {//if the record is not logically deleted
            head->body->Bloc[blockNum].Data[recordNum].deleted = true;
            printf("Record with ID %d logically deleted.\n", recordID);
        } else {
            printf("Record with ID %d is already logically deleted.\n", recordID);
        }
    }
}


/*----------------------------------------------------------------------------------------------------*/





/*-------------------------------------- Mehdi's Part --------------------------------------------------*/

void DeleteFile(char name[50], int numoffile, MsHead* head){

    if (head == NULL || head->meta == NULL) {        //head ou head->meta ne sont pas allouée donc il ne peut pas y'avoir de fichier

        printf("Error, invalid metadata structure.\n");

        return;

    }
    

    for (int i = 0; i < head->numberoffiles ; i++){

        if (strcmp(head->meta[i].name, name) == 0){

            if(head->numberoffiles == 1){       // Si il y'a qu'un seul élément libérer la mémoire allouée 

                free(head->meta);

                head->meta = NULL;      //NULL proposition ta3 chatgpt bech tiviti les pointeur "dangereux"

                head->numberoffiles = 0;

            }else{

                for (int j = i; j < numoffile-1; j++){    //Ecraser le fichier qu'on veut supprimer et decaler les autres fichiers

                    head->meta[j]=head->meta[j+1];

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




void PopulateFile(int filenumber, int numofrecord, MsHead* head, int* FB){

    bool exist = false;   //variable booleene initialiser a 'false' car on admet que le fichier n'existe pas

    if (head == NULL || head->meta == NULL) {        //head ou head->meta ne sont pas allouée donc il ne peut pas y'avoir de fichier

        printf("Error, invalid metadata structure.\n");

        return;

    }
    
    if (head->meta->filesizeRecord+numofrecord <= FB){     //3andna assez d'espace bech nzidou numofrecord
        
        for (int i = 0; i < head->numberoffiles; i++){     //boucle bech n7awes 3la file dyali
        
            if (filenumber == head->body->Bloc[i].id){
                
                exist = true;     //le fichier existe

                for (int j = 0; j < numofrecord; j++){
            
                    insertRecord(head,filenumber,rand() % 128,FB);   //insert numofrecord record w comme data random char
 
                }       
            
            }
        
        }

        if(exist){
        
            head->meta->filesizeRecord+=numofrecord;   //mise a jour du nombre de record dans le fichier

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


int main() {

// VARIABLES----------------

    FILE *MS; MS = fopen("MemoryS.data","rt+");

    Bloc Buffer;
    int NB, FB, organizationMode, interne,TaskChoice,filenumber, numoffile, numofrecord, IdOfFile;
    int* allocation = NULL;
    char name[50];
    char newname[50];
    MsHead* head = NULL;//head of the memory structure creato hna bch nkhdmo bih f ga3 le main

    do{
        printf("how many files do you want to store at maximum  :  ");
        scanf("%d", &numoffile);
        while (getchar() != '\n'); // Clear the input buffer
    }while(numoffile <= 0);

    int i = 0;
    
    struct File Files[numoffile];
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


      switch (TaskChoice)
      {
        case 1:
            printf("Initializing system...\n");
            initializeDisk(&NB, &FB, &organizationMode, &interne, &allocation);
            head = createMS(allocation, NB, FB , numoffile);
            break;
        case 2:
            printf("Creating a new file...\n");
            createfile(head,name,20, &organizationMode, &interne,&head->numberoffiles,&FB,&NB);
            break;
        case 3:
            printf("Please enter the ID of the file you want to automatically fill :\n");
            scanf("%d", &filenumber);
            printf("Can you please provide the number of record you want to insert :\n");
            scanf("%d",&numofrecord);
            PopulateFile(filenumber,numofrecord,head,FB);
        case 4:
            printf("Renaming a file...\n");
            printf("please enter the file number that you want to rename");
            scanf("%d", &filenumber);
            Renamefile(head,newname,filenumber,head->numberoffiles);
            break;
        case 5:
            printf("Adding a record...\n");
            printf("please enter the file number that you want to add a record");
            scanf("%d", &filenumber);
            printf("please enter the record data");
            scanf("%c", &Buffer.Data[0].data);
            insertRecord(head,filenumber,Buffer.Data[0].data,&FB);
            break;
        case 6:
            printf("Searching for record by ID...\n");
            printf("please enter the file number that you want to search a record: ");
            scanf("%d", &filenumber);
            printf("please enter the record ID: ");
            scanf("%d", &IdOfFile);
            int blockNum, recordNum;
            searchRecord(head, filenumber, IdOfFile, &FB, &blockNum, &recordNum);
            break;
        case 7:
            printf("Logically deleting a record...\n");
            printf("please enter the file number that you want to delete a record: ");
            scanf("%d", &filenumber);
            printf("please enter the record ID: ");
            scanf("%d", &IdOfFile);
            deleteRecord(head, filenumber, IdOfFile, &FB);
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
            fgets(name, 50, stdin);
            name[strcspn(name, "\n")] = '\0';
            puts("");
            DeleteFile(name, numoffile, head);
            break;
        case 13:
            printf("Clearing memory structure...\n");
            freeMS(head,NB);
            break;
        case 14:
            printf("Exiting the program. Goodbye!\n");
            exit(0);
        default:
            printf("Invalid choice. Please select a valid operation.\n");
            break;
      }  
    }
    

    /*
    initializeDisk(&NB, &FB, &organizationMode, &interne, &allocation);

    if (organizationMode == 1) {
        MsHead* head = createMS(allocation, NB, FB);

        if (head && head->alloca) {
            createfile(head,name,20, &organizationMode, &interne,&head->numberoffiles,&FB);
            Renamefile(head,newname,0,head->numberoffiles);
        } else {
            printf("Linked list creation failed or allocation table is empty.\n");
        }

        freeMS(head,NB);
    } else {
        printf("Contiguous organization not implemented yet.\n");
    }

    free(allocation);
    */
   
    return 0;
}