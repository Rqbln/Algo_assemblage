//
// Created by rqbin on 20/10/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>
#include <stdbool.h>
#include <string.h>
#ifndef ALGO_ASSEMBLAGE_SPROG_H
#define ALGO_ASSEMBLAGE_SPROG_H

#endif //ALGO_ASSEMBLAGE_SPROG_H

#define NUM_OPERATIONS 10  // Nombre total d'opérations
#define CYCLE_TIME 10      // Temps de cycle (en secondes)

typedef struct operation {
    char name[50];  // Nom de l'opération
    float duration; // Durée de l'opération
} t_operation;



typedef struct regleExclusion{
    int op1;  // Première opération
    int op2;  // Deuxième opération
} t_regleExclusion;

typedef struct Exclusion{
    int operationId;
    int stationId;
    int isActive; // 0 ou 1
} t_Exclusion;

typedef struct reglePrecedence{
    int op1;
    int op2;
} t_reglePrecedence;


t_regleExclusion* readExclusions(const char* filename, int* size);
t_reglePrecedence* readPrecedences(const char* filename, int* size);
t_operation* readOperations(const char* filename, int* size);
float readCycleTime(const char* filename);

void configureGLPK(glp_smcp *smcp, glp_iocp *iocp);
void solveAssemblyLineProblem(float cycleTime, int num_operations, t_operation* operations, t_regleExclusion* exclusions, t_reglePrecedence* precedences,int sizeExcl,int sizePrec, glp_smcp *smcp, glp_iocp *iocp);
