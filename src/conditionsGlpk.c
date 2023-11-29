//
// Created by rqbin on 22/11/23.
//
#include "sprog.h"

void configureGLPK(glp_smcp *smcp, glp_iocp *iocp) {
    glp_init_smcp(smcp);
    smcp->msg_lev = GLP_MSG_ON; // Active les messages pour glp_simplex

    glp_init_iocp(iocp);
    iocp->msg_lev = GLP_MSG_OFF; // Garde les messages désactivés pour glp_intopt
}

void solveAssemblyLineProblem(float cycleTime, int num_operations, t_operation* operations, t_regleExclusion* exclusions, t_reglePrecedence* precedences,int sizeExcl,int sizePrec, glp_smcp *smcp, glp_iocp *iocp) {
    // Création d'un nouveau problème linéaire.
    glp_prob *lp;
    lp = glp_create_prob();

    // Définition du nom du problème et de la direction de l'objectif (minimisation).
    glp_set_prob_name(lp, "assembly_line");
    glp_set_obj_dir(lp, GLP_MIN);

    // Création des variables de décision. Chaque variable représente une opération dans une station.
    printf("Nombre d'opérations : %d\n", num_operations);
    int numVars = num_operations * num_operations;
    printf("Nombre total de variables (numVars) : %d\n", numVars);
    glp_add_cols(lp, numVars);
    for (int i = 1; i <= num_operations; i++) {
        for (int j = 1; j <= num_operations; j++) {
            int idx = (i - 1) * num_operations + j; // Calcul de l'indice de la variable.
            glp_set_col_kind(lp, idx, GLP_BV); // Définition de la variable comme binaire.
            char name[20];
            sprintf(name, "x%d,%d", i, j); // Nommer la variable pour l'identification.
            glp_set_col_name(lp, idx, name);
            glp_set_obj_coef(lp, idx, operations[i - 1].duration); // Coefficient dans la fonction objectif (pour minimiser le nombre de stations).
        }
    }

    // Contraintes de précédence
    for (int k = 0; k < sizePrec; k++) {
        int opPre = precedences[k].op1;  // Opération précédente
        int opSub = precedences[k].op2;  // Opération subséquente
        for (int j = 1; j < num_operations; j++) {  // Pour chaque station, sauf la dernière
            for (int jp = j + 1; jp <= num_operations; jp++) {  // Pour chaque station après j
                int idx = glp_add_rows(lp, 1);
                glp_set_row_name(lp, idx, "precedence");
                glp_set_row_bnds(lp, idx, GLP_UP, -1.0, 0.0);  // opPre dans j doit précéder opSub dans jp
                int ind[3] = {0, (opPre - 1) * num_operations + j, (opSub - 1) * num_operations + jp};
                double val[3] = {0, 1.0, -1.0};
                glp_set_mat_row(lp, idx, 2, ind, val);
            }
        }
    }

    // Ajout des contraintes d'exclusion (deux opérations ne peuvent pas être dans la même station).
    for (int k = 0; k < sizeExcl; k++) {
        int op1 = exclusions[k].op1;
        int op2 = exclusions[k].op2;
        for (int j = 1; j <= num_operations; j++) {
            int idx = glp_add_rows(lp, 1);
            glp_set_row_name(lp, idx, "exclusion");
            glp_set_row_bnds(lp, idx, GLP_UP, 0.0, 1.0);
            int ind[3] = {0, (op1 - 1) * num_operations + j, (op2 - 1) * num_operations + j};
            double val[3] = {0, 1.0, 1.0};
            // Vérification des indices
            if (ind[1] > numVars || ind[2] > numVars) {
                printf("Erreur d'indice : ind[1] = %d, ind[2] = %d, numVars = %d\n", ind[1], ind[2], numVars);
            }
            glp_set_mat_row(lp, idx, 2, ind, val);
        }
    }




    // Ajout des contraintes de temps de cycle (la somme des durées des opérations dans une station ne doit pas dépasser le temps de cycle).
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

// Assurer l'Affectation des Opérations
    for (int i = 1; i <= num_operations; i++) {
        int idx = glp_add_rows(lp, 1);
        glp_set_row_name(lp, idx, "assignOperation");
        glp_set_row_bnds(lp, idx, GLP_LO, 1.0, 0.0);

        int ind[num_operations + 1]; // Déclaration du tableau ind
        double val[num_operations + 1]; // Déclaration du tableau val

        for (int j = 1; j <= num_operations; j++) {
            ind[j] = (i - 1) * num_operations + j;
            val[j] = 1.0;
        }
        glp_set_mat_row(lp, idx, num_operations, ind, val);
    }



    // Résolution du problème linéaire et entier.
    // Résolution du problème linéaire
    int simplex_status = glp_simplex(lp, smcp);
    if (simplex_status != 0) {
        printf("Problème lors de la résolution simplex : %d\n", simplex_status);
        // Gérer l'erreur
    }

    int intopt_status = glp_intopt(lp, iocp);
    if (intopt_status != 0) {
        printf("Problème lors de la résolution d'optimisation entière mixte : %d\n", intopt_status);
        // Gérer l'erreur
    }

// Vérification du statut de la solution pour MIP
    int sol_status = glp_mip_status(lp); // Utilisez glp_mip_status pour les solutions MIP
    switch (sol_status) {
        case GLP_OPT:
            printf("Solution MIP optimale trouvée\n");
            break;
            // Ajouter d'autres cas si nécessaire
    }

// Extraire les résultats pour les variables binaires
    if (sol_status == GLP_OPT) {
        double z = glp_mip_obj_val(lp); // Utilisez glp_mip_obj_val pour les solutions MIP
        printf("Valeur de la fonction objectif MIP : %f\n", z);

        for (int i = 1; i <= numVars; i++) {
            double val = glp_mip_col_val(lp, i); // Utilisez glp_mip_col_val pour les solutions MIP
            //printf("Variable MIP %d : %f\n", i, val);
        }
    }

// Calcul du temps total si les opérations étaient effectuées séquentiellement
    float totalTimeSequential = 0.0;
    for (int i = 0; i < num_operations; i++) {
        totalTimeSequential += operations[i].duration;
    }

    printf("\nPour un resultat minimal, les operations doivent etre agencees de cette facon :\n");
    int stationsUsed = 0;
    float totalTimeOptimized = 0.0; // Temps total sur toutes les stations avec l'optimisation.

    for (int j = 1; j <= num_operations; j++) {
        int operationsInStation = 0; // Compte le nombre d'opérations dans une station.
        float totalTimeInStation = 0.0; // Pour calculer le temps total par station.

        for (int i = 1; i <= num_operations; i++) {
            int idx = (i - 1) * num_operations + j; // Index de la variable.
            if (glp_mip_col_val(lp, idx) == 1) { // Vérifie si l'opération est assignée à cette station.
                if (operationsInStation == 0) {
                    printf("Station %d :\n", j);
                    stationsUsed++;
                }
                printf("%s ", operations[i - 1].name); // Affiche le nom de l'opération.
                totalTimeInStation += operations[i - 1].duration; // Ajoute la durée de l'opération au total de la station.
                operationsInStation++;
            }
        }
        if (operationsInStation > 0) {
            printf(" (Temps total: %.2f s)\n", totalTimeInStation); // Affiche le temps total de la station.
            totalTimeOptimized += totalTimeInStation; // Ajoute au temps total optimisé.
        }
    }

    float timeSaved = totalTimeSequential - totalTimeOptimized; // Calcul du temps gagné.

    printf("Pour conclure, nous aurons donc besoin de minimum %d stations avec un temps total de %.2f secondes.\n", stationsUsed, totalTimeOptimized);
    //printf("En effectuant chaque tâche à la chaîne, le temps total serait de %.2f secondes, d'où un gain de temps de %.2f secondes avec notre méthode.\n", totalTimeSequential, timeSaved);


    // Libération des ressources allouées pour le problème.
    glp_delete_prob(lp);
}
