
#include "donneesEntrees.h"

// Fonction pour saisir les opérations
void saisirOperations(Operation operations[], int* nbOperations) {
    printf("Entrez le nombre d'operations: ");
    scanf("%d", nbOperations);

    for(int i = 0; i < *nbOperations; i++) {
        printf("Entrez le nom et la duree de l'operation %d (en secondes): ", i+1);
        scanf("%s %d", operations[i].nom, &operations[i].duree);
    }
}

// Fonction pour saisir les contraintes d'exclusion
void saisirExclusions(int exclusions[][2], int* nbExclusions) {
    printf("Entrez le nombre de contraintes d'exclusion: ");
    scanf("%d", nbExclusions);

    for(int i = 0; i < *nbExclusions; i++) {
        printf("Entrez la paire d'operations (indices) qui ne peuvent pas etre a la meme station (exemple: 1 2): ");
        scanf("%d %d", &exclusions[i][0], &exclusions[i][1]);
    }
}

// Fonction pour saisir les contraintes de précédence
void saisirPrecedences(int precedences[][2], int* nbPrecedences) {
    printf("Entrez le nombre de contraintes de precedence: ");
    scanf("%d", nbPrecedences);

    for(int i = 0; i < *nbPrecedences; i++) {
        printf("Entrez l'ordre des operations (indices) ou la premiere doit preceder la seconde (exemple: 1 2): ");
        scanf("%d %d", &precedences[i][0], &precedences[i][1]);
    }
}

// Fonction pour saisir le temps de cycle
void saisirTempsCycle(int* tempsCycle) {
    printf("Entrez le temps de cycle (en secondes): ");
    scanf("%d", tempsCycle);
}

// Fonction principale qui utilise les sous-fonctions pour saisir toutes les données
void saisirDonnees() {
    Operation operations[MAX_OPERATIONS];
    int nbOperations;

    int exclusions[MAX_EXCLUSIONS][2];
    int nbExclusions;

    int precedences[MAX_PRECEDENCES][2];
    int nbPrecedences;

    int tempsCycle;

    // Saisir les opérations
    saisirOperations(operations, &nbOperations);

    // Saisir les exclusions
    saisirExclusions(exclusions, &nbExclusions);

    // Saisir les précédences
    saisirPrecedences(precedences, &nbPrecedences);

    // Saisir le temps de cycle
    saisirTempsCycle(&tempsCycle);

    // Après cela, vous pouvez utiliser les opérations, exclusions, précédences et le temps de cycle pour traiter l'optimisation de la ligne d'assemblage
}
