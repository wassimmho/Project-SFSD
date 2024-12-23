#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef struct enre {
    char data;       // Data
    int id;          // Block ID
    bool deleted;    // Logical deletion
} enre;

typedef struct Block {
    int id;          // Block ID
    enre* entries;   // Dynamic array of `enre`
    int next;        // Next block ID or -1 if last block
} Block;

typedef struct MSCH {
    struct Block* block;   // Dynamic array of blocks
} MSCH;

typedef struct Meta {
    char name[50];           // File name
    int filesizeBlock;       // File size in blocks
    int filesizeEnre;        // File size in entries
    int firstblockaddress;   // Address of the first block
    bool Global;             // Organization mode (1 = chained, 0 = contiguous)
    bool Intern;             // Sorted entries (1 = sorted, 0 = unsorted)
} Meta;

typedef struct MSheadCH {
    int* alloca;             // Allocation array
    Meta* meta;              // Meta data array
    MSCH* body;              // MSCH body
    int numberoffiles;
} MSheadCH;










//*********************************************************************************************************  Create scondary memory body and head ***********************************************
MSheadCH* createMS(int* allocat, int n, int m) {

    // Allocate memory for the head structure
    MSheadCH* head = (MSheadCH*)malloc(sizeof(MSheadCH));
    if (!head) {
        printf("Memory allocation failed for head\n");
        exit(1);
    }

    // Assign the allocation table and its size
    head->alloca = allocat;
    head->numberoffiles=0;
    // Allocate memory for the MSCH structure (body)
    MSCH* body = (MSCH*)malloc(sizeof(MSCH));
    if (!body) {
        printf("Memory allocation failed for body\n");
        free(head);
        exit(1);
    }

    // Allocate memory for the blocks array
    Block* blocks = (Block*)malloc(n * sizeof(Block));
    if (!blocks) {
        printf("Memory allocation failed for blocks\n");
        free(body);
        free(head);
        exit(1);
    }

    // Initialize each block
    for (int i = 0; i < n; i++) {
        blocks[i].id = i + 1;   // Assign block IDs
        blocks[i].next = -1;    // No next block by default

        // Allocate memory for entries in the block
        blocks[i].entries = (enre*)malloc(m * sizeof(enre));
        if (!blocks[i].entries) {
            printf("Memory allocation failed for entries in block %d\n", i + 1);
            for (int j = 0; j < i; j++) { // Free previous blocks
                free(blocks[j].entries);
            }
            free(blocks);
            free(body);
            free(head);
            exit(1);
        }

        // Initialize entries in the block
        for (int j = 0; j < m; j++) {
            blocks[i].entries[j].id = j + 1; // Assign entry IDs
            blocks[i].entries[j].data = '\0';    // No data initially
        }
    }

    // Assign blocks to the body
    body->block = blocks;

    // Link the body to the head
    head->body = body;

    printf("MS structure created with %d blocks, each containing %d entries.\n", n, m);
    return head;

}

//*********************************************************************************************************  initialise sorting modes allocation table ************************************************************** 
void initializeDisk(int* NB, int* FB, int* Orga, int* inter, int** vector) {
    do {
        printf("Enter the block size factor (> 0): ");
        scanf("%d", FB);
    } while (*FB <= 0);

    do {
        printf("Enter the total number of blocks (> 0): ");
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

    *vector = (int*)malloc(*NB * sizeof(int));
    if (!*vector) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    memset(*vector, 0, *NB * sizeof(int));
    printf("Allocation table initialized with %d cells, all set to 0.\n", *NB);
}

//***************************************************************************************************************************************************** 
void freeMS(MSheadCH* head,int n) {
    if (head == NULL) return;

    // Free allocation table
    free(head->alloca);

    // Free metadata
    free(head->meta);

    // Free blocks
    if (head->body != NULL) {
        if (head->body->block != NULL) {
            for (int i = 0; i < n; i++) {
                free(head->body->block[i].entries);
            }
            free(head->body->block);
        }
        free(head->body);
    }

    // Free head
    free(head);
}

void createfile(MSheadCH* head,char name[50],int NOR ,int* GO, int* IO,int* filenumber,int* FB){

        head->numberoffiles=head->numberoffiles+1;

        while (getchar() != '\n');// clear buffer tferegh buffer mour masste3melna scanf prs scanf y 5eli mourah \n li yreje3 fget tedi hadak character w te5roj bla mate9ra input donc like nta derte entrer fget te9ra hadik entrer li de5eltha nta mo9bil w tssoti direkte

        printf("woul you provide the file's name : ");
        fgets(name, 50, stdin); // scanf mais bach te9ra the hole line with space ( 3efssa te3 buffer)
        name[strcspn(name, "\n")] = '\0';
        puts("");

        strcpy(head->meta[*filenumber].name, name); 
        head->meta[*filenumber].filesizeBlock =(int)ceil((double)NOR / *FB);
        head->meta[*filenumber].filesizeEnre =NOR;
        head->meta[*filenumber].Global=*GO;
        head->meta[*filenumber].Intern=*IO;

       
}

int Renamefile (MSheadCH* head,char newname[50], int filenumber, int numoffiles){


    if (filenumber < 0 || filenumber >= numoffiles) {
        printf("Error: Invalid file number.\n");
        return -1;  // Invalid file number
    }
        
        printf("would you provide the new file's name :");
        fgets(newname, 50, stdin);
        newname[strcspn(newname, "\n")] = '\0';

        puts("");// tssouti ster

    for (int i = 0; i < numoffiles; i++) {
        if (i != filenumber && strcmp(head->meta[i].name, newname) == 0) {
            printf("Error: A file with the name '%s' already exists.\n", newname);
            return -1;  // Duplicate name found
        }
    }


strcpy(head->meta[filenumber].name, newname); //rename the file 
printf("File has been successfully renamed to '%s'.\n", newname);
    return 0;
}












//*********************************************************************************************************  Main function
int main() {

// VARIABLES--

    FILE *MC;
    int NB, FB, organizationMode, interne,desition,filenumber;
    int* allocation = NULL;
    char name[50];
    char newname[50];
    while (true)
    {
    printf("Please select an operation:\n");
    printf("1 - Initialize system\n");
    printf("2 - Create a new file\n");
    printf("3 - Rename a file\n");
    printf("4 - Add a record\n");
    printf("5 - Search for record by ID\n");
    printf("6 - Logically delete a record\n");
    printf("7 - Physically delete a record\n");
    printf("8 - Defragmentation\n");
    printf("9 - Print file content\n");
    printf("10 - Save file to disk\n");
    printf("11 - Delete a file\n");
    printf("12 - Clear memory structure\n");
    printf("13 - Exit\n");

    scanf("%d", &desition);

      switch (desition)
      {
      case 1:
            printf("Initializing system...\n");
            initializeDisk(&NB, &FB, &organizationMode, &interne, &allocation);
            MSheadCH* head = createMS(allocation, NB, FB);
            break;
        case 2:
            printf("Creating a new file...\n");
            createfile(head,name,20, &organizationMode, &interne,&head->numberoffiles,&FB);
            break;
        case 3:
            printf("Renaming a file...\n");
            printf("please enter the file number that you want to rename");
            scanf("%d", &filenumber);
            Renamefile(head,newname,filenumber,head->numberoffiles);
            break;
        case 4:
            printf("Adding a record...\n");
            break;
        case 5:
            printf("Searching for record by ID...\n");
            break;
        case 6:
            printf("Logically deleting a record...\n");
            break;
        case 7:
            printf("Physically deleting a record...\n");
            break;
        case 8:
            printf("Defragmenting...\n");
            break;
        case 9:
            printf("Printing file content...\n");
            break;
        case 10:
            printf("Saving file to disk...\n");
            break;
        case 11:
            printf("Deleting a file...\n");
            break;
        case 12:
            printf("Clearing memory structure...\n");
            freeMS(head,NB);
            break;
        case 13:
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
        MSheadCH* head = createMS(allocation, NB, FB);

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
