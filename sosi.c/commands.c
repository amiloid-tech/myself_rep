#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "commands.h"
#include "variables.h"
#include "io.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(x) usleep((x) * 1000)
#endif

extern char codeBuffer[MAXLINES][256];
extern int codeLines;

static void delaydots(const char* message)
{
    printf("%s", message);
    fflush(stdout);
    for (int i = 0; i < 3; i++) {
#ifdef _WIN32
        Sleep(400);
#else
        usleep(400000);
#endif
        printf(".");
        fflush(stdout);
    }
    printf("\n");
}

void execcmd(const char* line)
{
    if (!line) return;

    if (strncmp(line, "savef ", 6) == 0) {
        const char* fname = line + 6;
        savecode(fname);
    } else if (strncmp(line, "loadf ", 6) == 0) {
        const char* fname = line + 6;
        loadcode(fname);
    } else {
        printf("Unknown command: %s\n", line);
    }
}

void execline(const char* line)
{
    if (!line) return;

    char tmp[256];
    strncpy(tmp, line, sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';

    char* token = strtok(tmp, " ");
    if (!token) return;

    if (strcmp(token, "set") == 0) {
        char* var = strtok(NULL, " ");
        char* val = strtok(NULL, " ");
        if (var && val) {
            setvar(var, atoi(val));
        }
    } else if (strcmp(token, "print") == 0) {
        char* rest = strtok(NULL, "");
        if (rest) printvar(rest);
    } else if (strcmp(token, "arr") == 0) {
        char* name = strtok(NULL, " ");
        char* rest = strtok(NULL, "");
        if (name && rest) makearr(name, rest);
    } else if (strcmp(token, "adarr") == 0) {
        char* name = strtok(NULL, " ");
        char* val = strtok(NULL, " ");
        if (name && val) addarr(name, atoi(val));
    } else if (strcmp(token, "suarr") == 0) {
        char* name = strtok(NULL, " ");
        char* val = strtok(NULL, " ");
        if (name && val) delarr(name, atoi(val));
    } else if (strcmp(token, "savef") == 0 || strcmp(token, "loadf") == 0) {
        execcmd(line);
    } else {
        printf("Unknown instruction: %s\n", token);
    }
}

void execcode()
{
    delaydots("Executing code");
    for (int i = 0; i < codeLines; i++) {
        char line[256];
        strncpy(line, codeBuffer[i], sizeof(line) - 1);
        line[sizeof(line) - 1] = '\0';

        char* token = strtok(line, " ");
        if (!token) continue;

        if (strcmp(token, "sloop") == 0) {
            char* countstr = strtok(NULL, " ");
            if (!countstr) {
                printf("Error: sloop requires a number\n");
                continue;
            }
            int count = atoi(countstr);
            int start = i + 1;
            int end = -1;
            for (int j = start; j < codeLines; j++) {
                if (strncmp(codeBuffer[j], "eloop", 5) == 0) {
                    end = j;
                    break;
                }
            }
            if (end == -1) {
                printf("Error: sloop without eloop\n");
                break;
            }
            for (int r = 0; r < count; r++) {
                for (int j = start; j < end; j++) {
                    execline(codeBuffer[j]);
                }
            }
            i = end;
        } else {
            execline(codeBuffer[i]);
        }
    }
    printf("Code execution complete\n");
}
