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


// Définition du nombre total de variables d'ordre
    int num_order_vars = (num_operations * (num_operations - 1) / 2) * num_operations;
    glp_add_cols(lp, numVars + num_order_vars);

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




// Ajout des contraintes de précédence
    for (int k = 0; k < sizePrec; k++) {
        int opPre = precedences[k].op1;
        int opSub = precedences[k].op2;

        for (int j = 1; j <= num_operations; j++) {
            int idx1 = glp_add_rows(lp, 1);
            char row_name1[50];
            sprintf(row_name1, "precedence_%d_%d_station_%d", opPre, opSub, j);
            glp_set_row_name(lp, idx1, row_name1);
            glp_set_row_bnds(lp, idx1, GLP_UP, 0.0, 0.0);  // Utilisez GLP_UP pour la borne supérieure

            // Inversion du calcul de pairIndex pour refléter la nouvelle logique de précédence
            int pairIndex = (opSub - 1) * num_operations + (opPre - 1);
            int orderVarIndex = order_var_index + pairIndex * num_operations + j;

            // Vérifier que orderVarIndex est dans la plage valide
            if (orderVarIndex < numVars + 1 || orderVarIndex > numVars + num_order_vars) {
                continue; // Indice de variable d'ordre invalide
            }

            int ind1[3] = {0, (opSub - 1) * num_operations + j, orderVarIndex};
            double val1[3] = {0, 1.0, -1.0};  // Ajustement des valeurs pour la nouvelle logique
            glp_set_mat_row(lp, idx1, 2, ind1, val1);
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

        printf("Temps total optimisé : %.2f secondes (contre %.2f secondes en séquentiel).\n", totalTimeOptimized, totalTimeSequential);
    }

    // Libération des ressources allouées pour le problème.
    glp_delete_prob(lp);
}