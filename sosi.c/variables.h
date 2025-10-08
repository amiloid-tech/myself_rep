#ifndef VARIABLES_H
#define VARIABLES_H

#include <stdbool.h>

#define MAXVARS 100
#define MAXARRSIZE 100

typedef struct {
    char name[32];
    int value;
    bool isArray;
    int arr[MAXARRSIZE];
    int arrSize;
} Var;

extern Var varList[MAXVARS];
extern int varCount;

Var* findvar(const char* name);
void setvar(const char* name, int value);
void makearr(const char* name, char* values);
void addarr(const char* name, int value);
void delarr(const char* name, int value);
void printvar(const char* name);

#endif
