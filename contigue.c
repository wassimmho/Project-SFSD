#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Définition des structures pour gérer la mémoire secondaire et les fichiers
typedef struct enre {
    int dep;
    char data[100];
} enre;

typedef struct Block {
    int id;           // ID du bloc
    enre *entries;    // Tableau dynamique d'enregistrements
} Block;

typedef struct MS {
    int* alloca;      // Table d'allocation pour la mémoire secondaire
    Block* blocks;    // Tableau de blocs
} MShead;

typedef struct Meta {
    char name[50];
    int filesizeBlock;
    int filesizeEnre;
    Block* firstblockaddress;
    bool Global;
    bool Intern;
} Meta;

// Fonction de création de la mémoire secondaire en mode contigu
MShead* createContiguousMemory(int *allocat, int n, int m) {
    // Création du noeud de tête 
    MShead* head = (MShead*)malloc(sizeof(MShead));
    if (!head) {
        printf("Échec de l'allocation de mémoire\n");
        exit(1);
    }

    // Initialisation des champs de la mémoire secondaire
    head->alloca = allocat;
    head->blocks = (Block*)malloc(n * sizeof(Block)); // Allocation du tableau de blocs

    if (!head->blocks) {
        printf("Échec de l'allocation de mémoire pour les blocs\n");
        exit(1);
    }

    // Création des blocs de mémoire secondaire 
    for (int i = 0; i < n; i++) {
        head->blocks[i].id = i + 1; // Assigner un ID unique à chaque bloc
        head->blocks[i].entries = (enre*)malloc(m * sizeof(enre)); // Allocation des enregistrements pour chaque bloc

        if (!head->blocks[i].entries) {
            printf("Échec de l'allocation de mémoire pour les enregistrements\n");
            exit(1);
        }

        // Initialiser les enregistrements du bloc
        for (int j = 0; j < m; j++) {
            head->blocks[i].entries[j].dep = j + 1;
        }
    }

    return head; // Retourner l'adresse du noeud de tête de la mémoire
}

// Fonction d'initialisation du disque avec nombre de blocs et facteur de taille des blocs
void initializeDisk(int *NB, int *FB, int **vector) {
    int i = 0;
    do {
        if (i >= 2) {
            printf("Assurez-vous que le nombre total de blocs et le facteur de taille de bloc sont tous deux >= 1\n");
        }

        printf("Entrez le nombre total de blocs : ");
        scanf("%d", NB);
        printf("Entrez le facteur de taille des blocs : ");
        scanf("%d", FB);

        i++;
    } while (*FB <= 0 || *NB <= 0);  // Correction ici
    
    *vector = (int*)malloc(*NB * sizeof(int)); // Allocation de la table d'allocation pour la mémoire secondaire
    if (*vector == NULL) {
        printf("Échec de l'allocation de mémoire\n");
        exit(1);
    }

    memset(*vector, 0, *NB * sizeof(int)); // Initialisation de la table d'allocation avec des zéros
    printf("Table d'allocation initialisée avec %d cellules, toutes à 0.\n", *NB);
}

void freeMemory(MShead* head, int nb) {
    if (head == NULL) {
        return; // Rien à libérer
    }

    for (int i = 0; i < nb; i++) {
        free(head->blocks[i].entries); // Libérer les entrées du bloc
    }
    free(head->blocks); // Libérer les blocs
    free(head); // Libérer la tête de la mémoire secondaire
}


int main() {
    // Variables
    int NB, FB;
    int *allocation = NULL;

    // Initialiser le disque
    initializeDisk(&NB, &FB, &allocation);

    // Créer la mémoire secondaire en mode contigu
    MShead* head = createContiguousMemory(allocation, NB, FB);
    
    // Afficher et accéder au premier élément de la table d'allocation
    if (head != NULL && head->alloca != NULL) {
        printf("Première valeur dans alloca : %d\n", head->alloca[0]); // Afficher la première cellule de la table
    } else {
        printf("Échec de la création de la mémoire ou 'alloca' est vide.\n");
    }

    // Libérer la mémoire allouée
    free(allocation);
    freeMemory(head,NB); // Libérer toute la mémoire

    return 0;
}