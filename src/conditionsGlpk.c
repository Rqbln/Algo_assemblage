//
// Created by rqbin on 22/11/23.
//
#include "sprog.h"

void configureGLPK(glp_smcp *smcp, glp_iocp *iocp) {
    glp_init_smcp(smcp);
    smcp->msg_lev = GLP_MSG_ON; // Active les messages pour glp_simplex

    glp_init_iocp(iocp);
    iocp->msg_lev = GLP_MSG_ON; // Garde les messages désactivés pour glp_intopt
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


    // Définition du nombre total de variables d'ordre
    int num_order_vars = (num_operations * (num_operations - 1) / 2) * num_operations;
    glp_add_cols(lp, numVars + num_order_vars);

// Cette partie du code calcule le nombre total de variables d'ordre nécessaires.
// Pour chaque paire d'opérations (sauf les paires où les opérations sont identiques),
// et pour chaque station, une variable d'ordre est créée.
// Ces variables d'ordre serviront à indiquer si une opération est effectuée avant une autre dans la même station.
// Puis, ce nombre est utilisé pour ajouter les colonnes correspondantes au problème GLPK.

// Initialisation des variables d'ordre
    int order_var_index = numVars + 1;
    for (int i = 1; i <= num_operations; i++) {
        for (int j = 1; j <= num_operations; j++) {
            if (i == j) continue; // Pas de variable d'ordre pour la même opération
            for (int k = 1; k <= num_operations; k++) {
                int idx = order_var_index + (i - 1) * num_operations * (num_operations - 1) / 2 + (j - 1) * num_operations + k;
                glp_set_col_kind(lp, idx, GLP_BV);
                char name[20];
                sprintf(name, "y%d,%d,%d", i, j, k);
                glp_set_col_name(lp, idx, name);
            }
        }
    }

// Ici, on initialise les variables d'ordre. Pour chaque paire d'opérations distinctes (i et j) et chaque station (k),
// une variable binaire est créée pour déterminer si l'opération i précède l'opération j dans la station k.
// 'order_var_index' est l'index de départ pour ces variables d'ordre dans le modèle GLPK.
// Les variables sont nommées 'yijk' où i et j sont les indices des opérations et k est l'indice de la station.

// Ajout des contraintes d'ordre pour assurer que op1 est exécuté avant op2
    for (int k = 0; k < sizePrec; k++) {
        int op1 = precedences[k].op1;
        int op2 = precedences[k].op2;

        for (int j = 1; j <= num_operations; j++) {
            // Contrainte pour s'assurer que si op1 et op2 sont dans la même station j, alors op1 doit être exécuté avant op2
            int idx = glp_add_rows(lp, 1);
            char row_name[50];
            sprintf(row_name, "order_%d_before_%d_station_%d", op1, op2, j);
            glp_set_row_name(lp, idx, row_name);
            glp_set_row_bnds(lp, idx, GLP_LO, -1.0, 0.0); // Utilisation d'une borne inférieure

            // Variables : x[op1,j], x[op2,j], y[op1,op2,j]
            int ind[4] = {0, (op1 - 1) * num_operations + j, (op2 - 1) * num_operations + j, order_var_index + (op1 - 1) * num_operations * (num_operations - 1) / 2 + (op2 - 1) * num_operations + j};
            double val[4] = {0, 1.0, -1.0, 1.0};
            glp_set_mat_row(lp, idx, 3, ind, val);
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

// Après avoir trouvé la solution optimale
    if (sol_status == GLP_OPT) {
        double z = glp_mip_obj_val(lp); // Utilisez glp_mip_obj_val pour les solutions MIP
        printf("Valeur de la fonction objectif MIP : %f\n", z);

        // Identifiez les stations utilisées
        bool stationUsed[num_operations + 1];
        for (int i = 0; i <= num_operations; i++) {
            stationUsed[i] = false;
        }
        for (int i = 1; i <= numVars; i++) {
            if (glp_mip_col_val(lp, i) > 0.5) { // Si l'opération est assignée à cette station
                int station = (i - 1) % num_operations + 1;
                stationUsed[station] = true;
            }
        }

        // Créer un mappage des numéros de station
        int stationMapping[num_operations + 1];
        int currentStation = 1;
        for (int j = 1; j <= num_operations; j++) {
            if (stationUsed[j]) {
                stationMapping[j] = currentStation++;
            }
        }

        // Calcul du temps total si les opérations étaient effectuées séquentiellement
        float totalTimeSequential = 0.0;
        for (int i = 0; i < num_operations; i++) {
            totalTimeSequential += operations[i].duration;
        }

        printf("\nPour un resultat minimal, les operations doivent etre agencees de cette facon :\n");
        float totalTimeOptimized = 0.0; // Temps total sur toutes les stations avec l'optimisation.

        // Afficher les résultats avec les nouveaux numéros de station
        for (int j = 1; j <= num_operations; j++) {
            if (!stationUsed[j]) continue;

            int operationsInStation = 0; // Compte le nombre d'opérations dans une station.
            float totalTimeInStation = 0.0; // Pour calculer le temps total par station.

            for (int i = 1; i <= num_operations; i++) {
                int idx = (i - 1) * num_operations + j; // Index de la variable.
                if (glp_mip_col_val(lp, idx) == 1) { // Vérifie si l'opération est assignée à cette station.
                    if (operationsInStation == 0) {
                        printf("Station %d :\n", stationMapping[j]);
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

        printf("Temps total : %.2f secondes\n", totalTimeOptimized);
    }

    // Libération des ressources allouées pour le problème.
    glp_delete_prob(lp);
}