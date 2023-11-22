//
// Created by rqbin on 20/10/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>

#ifndef ALGO_ASSEMBLAGE_SPROG_H
#define ALGO_ASSEMBLAGE_SPROG_H

#endif //ALGO_ASSEMBLAGE_SPROG_H

typedef struct operation {
    int id;       // Identifiant de l'opération
    float duration; // Durée de l'opération, en float
} t_operation;



typedef struct exclusion{
    int op1;  // Première opération
    int op2;  // Deuxième opération
} t_exclusion;


typedef struct precedence{
    int precedent;  // Opération précédente
    int subsequent; // Opération subséquente
} t_precedence;


void afficherMenu();
void afficherMatrice(int **matrice, int nbLignes, int nbColonnes);

t_exclusion* readExclusions(const char* filename, int* size);
t_precedence* readPrecedences(const char* filename, int* size);
t_operation* readOperations(const char* filename, int* size);
float readCycleTime(const char* filename);

