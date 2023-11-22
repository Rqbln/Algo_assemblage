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
        printf("\nMenu de la chaine d'assemblage\n");
        printf("1. Afficher les exclusions\n");
        printf("2. Afficher les precedences\n");
        printf("3. Afficher les operations\n");
        printf("4. Afficher le temps de cycle\n");
        printf("5. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choice);

        // Traitement des choix
        switch(choice) {
            case 1:
                for (int i = 0; i < sizeExcl; i++) {
                    printf("%d) Exclusion : %d - %d\n",i,exclusions[i].op1, exclusions[i].op2);
                }
                break;
            case 2:
                for (int i = 0; i < sizePrec; i++) {
                    printf("%d) Precedence : %d -> %d\n",i,precedences[i].op1, precedences[i].op2);
                }
                break;
            case 3:
                for (int i = 0; i < sizeOps; i++) {
                    printf("Nom : %s  Duree = %f\n", operations[i].name, operations[i].duration);
                }
                break;
            case 4:
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
