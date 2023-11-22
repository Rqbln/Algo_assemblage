#include <stdio.h>
#include <stdlib.h>
#include "sprog.h" // Assurez-vous d'inclure vos structures et fonctions ici

int main() {
    // Chargement des données
    int sizeExcl, sizePrec, sizeOps;
    t_exclusion* exclusions = readExclusions("../Donnees/exclusions.txt", &sizeExcl);
    t_precedence* precedences = readPrecedences("../Donnees/precedences.txt", &sizePrec);
    t_operation* operations = readOperations("../Donnees/operations.txt", &sizeOps);
    float cycleTime = readCycleTime("../Donnees/temps_cycle.txt");

    int choice;

    do {
        // Affichage du menu
        printf("\nMenu de la chaîne d'assemblage\n");
        printf("1. Afficher les exclusions\n");
        printf("2. Afficher les précédences\n");
        printf("3. Afficher les opérations\n");
        printf("4. Afficher le temps de cycle\n");
        printf("5. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choice);

        // Gestion des choix
        switch(choice) {
            case 1:
                // Affichage des exclusions
                for (int i = 0; i < sizeExcl; i++) {
                    printf("Exclusion: %d - %d\n", exclusions[i].op1, exclusions[i].op2);
                }
                break;
            case 2:
                // Affichage des précédences
                for (int i = 0; i < sizePrec; i++) {
                    printf("Précédence: %d -> %d\n", precedences[i].precedent, precedences[i].subsequent);
                }
                break;
            case 3:
                // Affichage des opérations
                for (int i = 0; i < sizeOps; i++) {
                    printf("Opération %d: Durée = %f\n", operations[i].id, operations[i].duration);
                }
                break;
            case 4:
                // Affichage du temps de cycle
                printf("Temps de cycle: %f\n", cycleTime);
                break;
            case 5:
                printf("Quitter\n");
                break;
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }
    } while (choice != 5);

    // Libération de la mémoire
    free(exclusions);
    free(precedences);
    free(operations);

    return 0;
}
