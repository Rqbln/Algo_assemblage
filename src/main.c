#include "sprog.h"

/*
 * Objectif :
 * Maximiser la fonction objectif Z = x + y.
 *
 * Contraintes :
 * 1. La somme de x et du double de y doit être inférieure ou égale à 10 : x + 2y <= 10.
 * 2. La somme du triple de x et de y doit être inférieure ou égale à 12 : 3x + y <= 12.
 * 3. Les variables x et y doivent être positives ou nulles : x >= 0, y >= 0.
 *
 * Description :
 * Dans ce problème, nous cherchons à maximiser la somme de deux variables x et y
 * sous des contraintes linéaires. Les contraintes limitent les valeurs que x et y
 * peuvent prendre. Le solveur GLPK utilisera l'algorithme du simplex pour trouver
 * les valeurs de x et y qui maximisent Z tout en respectant les contraintes énoncées.
 */


int main(void) {
    glp_prob *lp;
    int ia[1+1000], ja[1+1000];
    double ar[1+1000], z, x, y;

    // Création d'un problème
    lp = glp_create_prob();
    glp_set_prob_name(lp, "sample");
    glp_set_obj_dir(lp, GLP_MAX);

    // Ajouter des lignes (contraintes)
    glp_add_rows(lp, 2);
    glp_set_row_name(lp, 1, "p");
    glp_set_row_bnds(lp, 1, GLP_UP, 0.0, 10.0);  // x + 2y <= 10
    glp_set_row_name(lp, 2, "q");
    glp_set_row_bnds(lp, 2, GLP_UP, 0.0, 12.0);  // 3x + y <= 12

    // Ajouter des colonnes (variables)
    glp_add_cols(lp, 2);
    glp_set_col_name(lp, 1, "x");
    glp_set_col_bnds(lp, 1, GLP_LO, 0.0, 0.0);
    glp_set_obj_coef(lp, 1, 1.0);  // Coefficient de x dans la fonction objectif
    glp_set_col_name(lp, 2, "y");
    glp_set_col_bnds(lp, 2, GLP_LO, 0.0, 0.0);
    glp_set_obj_coef(lp, 2, 1.0);  // Coefficient de y dans la fonction objectif

    // Définir les coefficients des contraintes
    ia[1] = 1, ja[1] = 1, ar[1] = 1.0;  // 1ère ligne, 1ère colonne
    ia[2] = 1, ja[2] = 2, ar[2] = 2.0;  // 1ère ligne, 2ème colonne
    ia[3] = 2, ja[3] = 1, ar[3] = 3.0;  // 2ème ligne, 1ère colonne
    ia[4] = 2, ja[4] = 2, ar[4] = 1.0;  // 2ème ligne, 2ème colonne

    // Charger la matrice des contraintes
    glp_load_matrix(lp, 4, ia, ja, ar);

    // Résoudre le problème
    glp_simplex(lp, NULL);

    // Récupérer et afficher la solution
    z = glp_get_obj_val(lp);
    x = glp_get_col_prim(lp, 1);
    y = glp_get_col_prim(lp, 2);

    printf("z = %g; x = %g; y = %g\n", z, x, y);

    // Libérer la mémoire
    glp_delete_prob(lp);

    return 0;
}

/*
 GLPK Simplex Optimizer, v4.65
2 rows, 2 columns, 4 non-zeros
*     0: obj =  -0.000000000e+00 inf =   0.000e+00 (2)
*     2: obj =   6.400000000e+00 inf =   0.000e+00 (0)
OPTIMAL LP SOLUTION FOUND
z = 6.4; x = 2.8; y = 3.6
 */
