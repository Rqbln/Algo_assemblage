#include <stdio.h>

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

int main() {
    int choix;
    int **matrice1, **matrice2, **matrice3;
    int nbLignes1, nbLignes2, nbLignes3; // À initialiser
    int nbColonnes1, nbColonnes2, nbColonnes3; // À initialiser

    // Ici, vous pouvez inclure le code pour lire et remplir les matrices.

    do {
        afficherMenu();
        scanf("%d", &choix);

        switch(choix) {
            case 1:
                afficherMatrice(matrice1, nbLignes1, nbColonnes1);
                break;
            case 2:
                afficherMatrice(matrice2, nbLignes2, nbColonnes2);
                break;
            case 3:
                afficherMatrice(matrice3, nbLignes3, nbColonnes3);
                break;
            case 4:
                printf("Quitter le programme.\n");
                break;
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }
    } while (choix != 4);

    // Ici, libérez les ressources allouées, si nécessaire.

    return 0;
}
