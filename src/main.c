#include <stdio.h>
#include <stdlib.h>
#include "sprog.h" // Assurez-vous d'inclure vos structures et fonctions ici

int main() {
    // Chargement des données
    int sizeExcl, sizePrec, sizeOps;
    t_regleExclusion * regleExclusions = readExclusions("../Donnees/exclusions.txt", &sizeExcl);
    t_reglePrecedence * reglePrecedences = readPrecedences("../Donnees/precedences.txt", &sizePrec);
    t_operation* operations = readOperations("../Donnees/operations.txt", &sizeOps);
    float cycleTime = readCycleTime("../Donnees/temps_cycle.txt");

    // Structures pour les paramètres
    glp_smcp smcp;
    glp_iocp iocp;

    // Configurer GLPK pour désactiver les messages de sortie
    configureGLPK(&smcp, &iocp);

    int choice;

    do {
        // Affichage du menu
        printf("\nMenu de la chaine d'assemblage\n");
        printf("1. Afficher les exclusions\n");
        printf("2. Afficher les precedences\n");
        printf("3. Afficher les operations\n");
        printf("4. Afficher le temps de cycle\n");
        printf("5. Calculer le nombre de stations minimal\n");
        printf("6. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choice);

        // Traitement des choix
        switch(choice) {
            case 1:
                for (int i = 0; i < sizeExcl; i++) {
                    printf("%d) Exclusion : %d - %d\n",i,regleExclusions[i].op1, regleExclusions[i].op2);
                }
                break;
            case 2:
                for (int i = 0; i < sizePrec; i++) {
                    printf("%d) Precedence : %d -> %d\n",i,reglePrecedences[i].op1, reglePrecedences[i].op2);
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
                solveAssemblyLineProblem(cycleTime, sizeOps, operations, regleExclusions, sizeExcl, &smcp, &iocp);

                break;
            case 6:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }
    } while (choice != 6);


    // Libération de la mémoire
    free(regleExclusions);
    free(reglePrecedences);
    free(operations);

    return 0;
}
