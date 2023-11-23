//
// Created by rqbin on 22/11/23.
//
#include "sprog.h"

t_regleExclusion * readExclusions(const char* filename, int* size) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open the file");
        exit(1);
    }

    t_regleExclusion * regleExclusions = malloc(sizeof(t_regleExclusion ) * 100); // Taille initiale
    int count = 0;

    while (fscanf(file, "%d %d", &regleExclusions[count].op1, &regleExclusions[count].op2) == 2) {
        count++;
        if (count % 100 == 0) {
            regleExclusions = realloc(regleExclusions, sizeof(t_regleExclusion ) * (count + 100));
        }
    }

    *size = count;
    fclose(file);
    return regleExclusions;
}


t_reglePrecedence * readPrecedences(const char* filename, int* size) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open the file");
        exit(1);
    }

    t_reglePrecedence * reglePrecedences = malloc(sizeof(t_reglePrecedence) * 100);
    int count = 0;

    while (fscanf(file, "%d %d", &reglePrecedences[count].op1, &reglePrecedences[count].op2) == 2) {
        count++;
        if (count % 100 == 0) {
            reglePrecedences = realloc(reglePrecedences, sizeof(t_reglePrecedence ) * (count + 100));
        }
    }

    *size = count;
    fclose(file);
    return reglePrecedences;
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
