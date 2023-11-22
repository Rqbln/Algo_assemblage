//
// Created by rqbin on 22/11/23.
//
#include "sprog.h"

t_exclusion* readExclusions(const char* filename, int* size) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open the file");
        exit(1);
    }

    t_exclusion* exclusions = malloc(sizeof(t_exclusion) * 100); // Taille initiale
    int count = 0;

    while (fscanf(file, "%d %d", &exclusions[count].op1, &exclusions[count].op2) == 2) {
        count++;
        if (count % 100 == 0) {
            exclusions = realloc(exclusions, sizeof(t_exclusion) * (count + 100));
        }
    }

    *size = count;
    fclose(file);
    return exclusions;
}


t_precedence* readPrecedences(const char* filename, int* size) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open the file");
        exit(1);
    }

    t_precedence* precedences = malloc(sizeof(t_precedence) * 100);
    int count = 0;

    while (fscanf(file, "%d %d", &precedences[count].op1, &precedences[count].op2) == 2) {
        count++;
        if (count % 100 == 0) {
            precedences = realloc(precedences, sizeof(t_precedence) * (count + 100));
        }
    }

    *size = count;
    fclose(file);
    return precedences;
}



t_operation* readOperations(const char* filename, int* size) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open the file");
        exit(1);
    }

    t_operation* operations = malloc(sizeof(t_operation) * 100);
    int count = 0;

    while (fscanf(file, "%s %f", operations[count].name, &operations[count].duration) == 2) {
        count++;
        if (count % 100 == 0) {
            operations = realloc(operations, sizeof(t_operation) * (count + 100));
        }
    }

    *size = count;
    fclose(file);
    return operations;
}





float readCycleTime(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open the file");
        exit(1);
    }

    float cycleTime;
    fscanf(file, "%f", &cycleTime);
    fclose(file);
    return cycleTime;
}
