#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//----------------------------------------------------------------------------------------------------------------------------------------------------
// Define `MSCH` first


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
} MSheadCH;

//*********************************************************************************************************  Create Function ***********************************************
MSheadCH* createMS(int* allocat, int n, int m) {

    // Allocate memory for the head structure
    MSheadCH* head = (MSheadCH*)malloc(sizeof(MSheadCH));
    if (!head) {
        printf("Memory allocation failed for head\n");
        exit(1);
    }

    // Assign the allocation table and its size
    head->alloca = allocat;

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

//************************************************************************************************************************************************************************************************************** 
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

//***************************************************************************************************************************************************** 
int main() {
    FILE *MC;
    int NB, FB, organizationMode, interne;
    int* allocation = NULL;

    // Initialize disk
    initializeDisk(&NB, &FB, &organizationMode, &interne, &allocation);

    if (organizationMode == 1) {
        MSheadCH* head = createMS(allocation, NB, FB);

        if (head && head->alloca) {
            printf("First value in allocation table: %d\n", head->alloca[0]);
        } else {
            printf("Linked list creation failed or allocation table is empty.\n");
        }

        freeMS(head,NB);
    } else {
        printf("Contiguous organization not implemented yet.\n");
    }

    free(allocation);
    return 0;
}
