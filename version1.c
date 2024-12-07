#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//----------------------------------------------------------------------------------------------------------------------------------------------------
typedef struct enre {
    int dep;
    char data[100]; // stam et moza
} enre;

typedef struct Block {
    int id;           // Block ID
    enre *entries;    // Dynamic array of enre
} Block;

typedef struct MSCHheadCH {
    int* alloca; 
    struct MSCH* next; //type raho MSCH prsq f fonction bach te5den list pointer MSCH maye9derch yedi 9ima te3 pointer MSCHheadCH donc bedelte bach ne9der ndir assignement
} MSCHheadCH;

typedef struct MSCH {
    Block block;
    struct MSCH* next;
} MSCH;

typedef struct Meta {
    char name[50];
    int filesizeBlock;
    int filesizeEnre;
    MSCH* firstblockaddress;
    bool Global;
    bool Intern;
} Meta;

//*********************************************************************************************************  CREAT FUNCTION ***********************************************
MSCHheadCH* createLinkedList(int *allocat, int n, int m) {
    //-------------------------------------------------------------------------------------------- Creat Head node  ------------------------------------------------
    MSCHheadCH* head = (MSCHheadCH*)malloc(sizeof(MSCHheadCH));
    if (!head) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    //-------------------------------------------------------------------------------------------- link allocation table + next pointer=NULL  ------------------------------------------------
    head->alloca = allocat;
    head->next = NULL;
    
    MSCH* NOADIN = (MSCH*)malloc(sizeof(MSCH));
    if (!NOADIN) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    //-------------------------------------------------------------------------------------------- Creat nodes  ------------------------------------------------
    for (int i = 0; i < n; i++) {
        MSCH* NOADnext = (MSCH*)malloc(sizeof(MSCH));
        if (!NOADnext) {
            printf("Memory allocation failed\n");
            exit(1);
        }

        //-------------------------------------------------------------------------------------------- ID OF Block + allocate structure array  ------------------------------------------------
        NOADnext->block.id = i + 1; // Assign a unique ID to each block
        NOADnext->block.entries = (enre*)malloc(m * sizeof(enre)); // Example: n entries per block
        if (!NOADnext->block.entries) {
            printf("Memory allocation for entries failed\n");
            exit(1);
        }

        //-------------------------------------------------------------------------------------------- ID OF STRUCTURES ------------------------------------------------
        for (int j = 0; j < m; j++) {
            NOADnext->block.entries[j].dep = j + 1;
        }

        //-------------------------------------------------------------------------------------------- Give last node next pointer value Null ( node le5er ge3 f la liste mayepwanti 3la walo) ------------------------------------------------
        NOADnext->next = NULL; // No next node yet

        //-------------------------------------------------------------------------------------------- 1/- Ylesse9 head m3a first node ------------------------------------------------
        //-------------------------------------------------------------------------------------------- 2/- ye5dem node w ylesse9ha direct m3a li 9belha  ------------------------------------------------
        //-------------------------------------------------------------------------------------------- 3/- ybedel la valeur te3 pointeur bach yweli current node hya node le5ra li 5demnaha   ------------------------------------------------
        if (i == 0) {
            head->next = NOADnext; // Link the first node to the head
        } else {
            NOADIN->next = NOADnext; // Link the previous node to the current one
        }

        NOADIN = NOADnext; // Move to the current node
    }

    return head;
}

//************************************************************************************************************************************************************************************************************** 
void initializeDisk(int *NB, int *FB, int *Orga, int **vector) {
    int i = 0;
    do {
        if (i >= 1) {
            printf("ensure that the block size factor is > or = 1 \n");
        }
        
        printf("Enter the block size factor: \n");
        scanf("%d", FB);

        i++;
    } while (*FB <= 0);

    i = 0;

    do {
        if (i >= 1) {
            printf("ensure that total number of blocks is > or = 1 \n");
        }
        
        printf("Enter the total number of blocks: \n");
        scanf("%d", NB);

        i++;
    } while (*NB <= 0);

    i = 0;

    do {
        if (i >= 1) {
            printf("Invalid input! Please enter **1** for **Chainer** or **0** for **Contigue**. \n");
        }
        
        printf("Please choose an organization method for your data:\n- Enter **1** for **Chainer**.\n- Enter **0** for **Contigue**.  \n");
        scanf("%d", Orga);

        i++;
    } while (*Orga != 0 && *Orga != 1);

    *vector = (int*)malloc(*NB * sizeof(int));
    if (*vector == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    memset(*vector, 0, *NB * sizeof(int));
    printf("allocation table initialized with %d cells, all set to 0.\n", *NB);
}

//----------------------------------------------------------------------------------------------------------
void freeLinkedList(MSCHheadCH* head) {
    if (head == NULL) {
        return; // Nothing to free
    }

    MSCH* current = head->next; 
    while (current != NULL) {
        MSCH* nextNode = current->next; 

        if (current->block.entries != NULL) {
            free(current->block.entries);
        }

        free(current);

        current = nextNode;
    }

    free(head);
}
//***************************************************************************************************************************************************** 

int main() {
    //Var---------------------------------------------------------------------------------------------------------------
    int NB, FB, organizationMode;
    int *allocation = NULL;

    // Initialize the disk--------------------------------------------------------------------------------------------------------
    initializeDisk(&NB, &FB, &organizationMode, &allocation);

    if (organizationMode == 1) {
        // Organization chainee
        MSCHheadCH* head = createLinkedList(allocation, NB, FB);
        
        // Access and print the first value of `alloca`
        if (head != NULL && head->alloca != NULL) { // Ensure `head` and `alloca` are valid
            printf("First value in allica: %d\n", head->alloca[0]); // Print the first cell content
        } else {
            printf("Linked list creation failed or `alloca` is empty.\n");
        }

        // Free allocated memory---------------------------------------------------------------------------------------------------------
        free(allocation);
        freeLinkedList(head);
    }else{
        // CONTIGUE
        printf("mo7ssin hna plassa te3 les fonctions ta3ek ida 7ebite");
    }

    return 0;
}
