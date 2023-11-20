//
// Created by rqbin on 20/10/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>

#ifndef ALGO_ASSEMBLAGE_SPROG_H
#define ALGO_ASSEMBLAGE_SPROG_H

#endif //ALGO_ASSEMBLAGE_SPROG_H

typedef struct operations{
    char poserCarrosserie[50];
    char monterRoue[50];
    char installerSiege[50];
    char monterMoteur[50];
    char poserCircuitElectrique[50];
    char peindreCarrosserie[50];
    char assemblerTableau[50];
    char monterFreins[50];
    char ajouterCompElectriques[50];
    int duration;
} t_operations;

typedef struct {
    char operation1[50];
    char operation2[50];
} Exclusion;

typedef struct {
    char precedent[50];
    char subsequent[50];
} Precedence;

