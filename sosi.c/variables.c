#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "variables.h"

Var varList[MAXVARS];
int varCount = 0;

Var* findvar(const char* name)
{
    for (int i = 0; i < varCount; i++) {
        if (strcmp(varList[i].name, name) == 0)
            return &varList[i];
    }
    return NULL;
}

void setvar(const char* name, int value)
{
    Var* v = findvar(name);
    if (!v) {
        if (varCount >= MAXVARS) {
            printf("Error: variable limit reached\n");
            return;
        }
        v = &varList[varCount++];
        strncpy(v->name, name, sizeof(v->name) - 1);
        v->name[sizeof(v->name) - 1] = '\0';
        v->isArray = false;
        v->arrSize = 0;
    }
    v->value = value;
}

void makearr(const char* name, char* values)
{
    Var* v = findvar(name);
    if (!v) {
        if (varCount >= MAXVARS) {
            printf("Error: variable limit reached\n");
            return;
        }
        v = &varList[varCount++];
        strncpy(v->name, name, sizeof(v->name) - 1);
        v->name[sizeof(v->name) - 1] = '\0';
        v->isArray = true;
        v->arrSize = 0;
    } else {
        v->isArray = true;
        v->arrSize = 0;
    }

    char* tok = strtok(values, ",");
    while (tok && v->arrSize < MAXARRSIZE) {
        v->arr[v->arrSize++] = atoi(tok);
        tok = strtok(NULL, ",");
    }
}

void addarr(const char* name, int value)
{
    Var* v = findvar(name);
    if (v && v->isArray && v->arrSize < MAXARRSIZE) {
        v->arr[v->arrSize++] = value;
    } else {
        printf("Error: cannot add element to array %s\n", name);
    }
}

void delarr(const char* name, int value)
{
    Var* v = findvar(name);
    if (v && v->isArray) {
        for (int i = 0; i < v->arrSize; i++) {
            if (v->arr[i] == value) {
                for (int j = i; j < v->arrSize - 1; j++)
                    v->arr[j] = v->arr[j + 1];
                v->arrSize--;
                break;
            }
        }
    } else {
        printf("Error: cannot remove element from array %s\n", name);
    }
}

void printvar(const char* name)
{
    if (!name || name[0] == '\0') return;

    if (name[0] == '"') {
        printf("%s\n", name + 1);
        return;
    }

    Var* v = findvar(name);
    if (!v) {
        printf("Unknown variable: %s\n", name);
        return;
    }

    if (v->isArray) {
        printf("[");
        for (int i = 0; i < v->arrSize; i++) {
            printf("%d", v->arr[i]);
            if (i < v->arrSize - 1) printf(", ");
        }
        printf("]\n");
    } else {
        printf("%d\n", v->value);
    }
}
