//
// Created by rqbin on 08/11/23.
//

#include <stdio.h>
#include <stdlib.h>

#ifndef ALGO_ASSEMBLAGE_DONNEESENTREES_H
#define ALGO_ASSEMBLAGE_DONNEESENTREES_H

#endif //ALGO_ASSEMBLAGE_DONNEESENTREES_H

#define MAX_OPERATIONS 50
#define MAX_EXCLUSIONS 100
#define MAX_PRECEDENCES 100


// Structure pour stocker les informations d'une opération
typedef struct {
    char nom[50];
    int duree;
} Operation;

void saisirOperations(Operation operations[], int* nbOperations);
void saisirExclusions(int exclusions[][2], int* nbExclusions);
void saisirPrecedences(int precedences[][2], int* nbPrecedences);
void saisirTempsCycle(int* tempsCycle);
void saisirDonnees();