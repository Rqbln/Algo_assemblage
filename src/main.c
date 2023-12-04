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
    char buffer[10]; // Définir un buffer pour l'entrée

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

        // Utilisation de fgets pour lire la ligne
        fgets(buffer, 10, stdin);

        // Conversion du buffer en entier et gestion des erreurs
        if (sscanf(buffer, "%d", &choice) != 1) {
            printf("Entree invalide. Veuillez entrer un nombre.\n");
            continue; // Continue sans traiter le reste du code dans la boucle
        }

        // Traitement des choix
        switch(choice) {
            case 1:
                // Votre code pour le cas 1
                break;
            case 2:
                // Votre code pour le cas 2
                break;
                // Ajoutez les autres cas ici
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
