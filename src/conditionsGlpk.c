//
// Created by rqbin on 22/11/23.
//
#include "sprog.h"

// Sous-programme pour initialiser la matrice d'exclusion
t_Exclusion** initExclusionMatrix(int numOperations, int numStations) {
    t_Exclusion** matrix = malloc(numOperations * sizeof(t_Exclusion*));
    for (int i = 0; i < numOperations; i++) {
        matrix[i] = malloc(numStations * sizeof(t_Exclusion));
        for (int j = 0; j < numStations; j++) {
            matrix[i][j].operationId = i + 1; // L'ID de l'opération, en commençant par 1
            matrix[i][j].stationId = j + 1;   // L'ID de la station, en commençant par 1
            matrix[i][j].isActive = 0;        // Initialisé à 0
        }
    }
    return matrix;
}

void solveAssemblyLineProblem(float cycleTime, int num_operations, t_operation* operations, t_regleExclusion* exclusions, int sizeExcl) {
    glp_prob *lp;
    lp = glp_create_prob();
    glp_set_prob_name(lp, "assembly_line");
    glp_set_obj_dir(lp, GLP_MIN);

    // Variables
    int numVars = num_operations * num_operations;
    glp_add_cols(lp, numVars);
    for (int i = 1; i <= num_operations; i++) {
        for (int j = 1; j <= num_operations; j++) {
            int idx = (i - 1) * num_operations + j;
            glp_set_col_kind(lp, idx, GLP_BV);
            char name[20];
            sprintf(name, "x%d,%d", i, j);
            glp_set_col_name(lp, idx, name);
            // Objectif : minimiser le nombre de stations utilisées
            glp_set_obj_coef(lp, idx, 1.0);
        }
    }

    // Contraintes d'exclusion
    for (int k = 0; k < sizeExcl; k++) {
        int op1 = exclusions[k].op1;
        int op2 = exclusions[k].op2;
        for (int j = 1; j <= num_operations; j++) {
            int idx = glp_add_rows(lp, 1);
            glp_set_row_name(lp, idx, "exclusion");
            glp_set_row_bnds(lp, idx, GLP_UP, 0.0, 1.0);
            int ind[3] = {0, (op1 - 1) * num_operations + j, (op2 - 1) * num_operations + j};
            double val[3] = {0, 1.0, 1.0};
            glp_set_mat_row(lp, idx, 2, ind, val);
        }
    }

    // Contraintes de temps de cycle
    for (int j = 1; j <= num_operations; j++) {
        int idx = glp_add_rows(lp, 1);
        char name[20];
        sprintf(name, "cycleTime%d", j);
        glp_set_row_name(lp, idx, name);
        glp_set_row_bnds(lp, idx, GLP_UP, 0.0, cycleTime);
        int ind[num_operations + 1];
        double val[num_operations + 1];
        for (int i = 1; i <= num_operations; i++) {
            ind[i] = (i - 1) * num_operations + j;
            val[i] = operations[i - 1].duration;
        }
        glp_set_mat_row(lp, idx, num_operations, ind, val);
    }

    // Résoudre le problème
    glp_simplex(lp, NULL);
    glp_intopt(lp, NULL);

    // Afficher les résultats
    printf("\nEn prenant en compte les exclusions :\n");
    for (int k = 0; k < sizeExcl; k++) {
        printf("%s et %s\n", operations[exclusions[k].op1 - 1].name, operations[exclusions[k].op2 - 1].name);
    }

    printf("\nPour un resultat minimal, les operations doivent etre agencees de cette façon :\n");
    int stationsUsed = 0;
    for (int j = 1; j <= num_operations; j++) {
        int operationsInStation = 0;
        printf("Station %d :\n", j);
        for (int i = 1; i <= num_operations; i++) {
            int idx = (i - 1) * num_operations + j;
            if (glp_mip_col_val(lp, idx) == 1) {
                printf("%s ", operations[i - 1].name);
                operationsInStation++;
            }
        }
        if (operationsInStation > 0) {
            printf("\n");
            stationsUsed++;
        }
    }

    printf("Pour conclure, nous aurons donc besoin de minimum %d stations\n", stationsUsed);

    // Libération de la mémoire
    glp_delete_prob(lp);
}