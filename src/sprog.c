//
// Created by rqbin on 20/10/23.
//

#include "sprog.h"

void afficherMatrice(int **matrice, int nbLignes, int nbColonnes) {
    for (int i = 0; i < nbLignes; i++) {
        for (int j = 0; j < nbColonnes; j++) {
            printf("%d ", matrice[i][j]);
        }
        printf("\n");
    }
}

void afficherMenu() {
    printf("Menu :\n");
    printf("1. Afficher la matrice du fichier 1\n");
    printf("2. Afficher la matrice du fichier 2\n");
    printf("3. Afficher la matrice du fichier 3\n");
    printf("4. Quitter\n");
    printf("Entrez votre choix : ");
}

