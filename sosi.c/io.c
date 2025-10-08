#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <stdio.h>
#include <conio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(x) usleep((x) * 1000)
#endif


#include "io.h"
#include "commands.h"

char codeBuffer[MAXLINES][256];
int codeLines = 0;

static const char* colorReset = "\x1b[0m";
static const char* colorTitle = "\x1b[36m";   /* cyan */
static const char* colorKey = "\x1b[33m";     /* yellow */
static const char* colorText = "\x1b[37m";    /* white */
static const char* colorAccent = "\x1b[35m";  /* magenta */
static const char* colorInfo = "\x1b[32m";    /* green */

void enableAnsi() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004;
    SetConsoleMode(hOut, dwMode);
#endif
}


int keyhit() {
#ifdef _WIN32
    return _kbhit();
#else
    return 0;
#endif
}



int keyget()
{
#ifdef _WIN32
    return _getch();
#else
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
#endif
}

static void loading(const char* message)
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

void printmenu()
{
    enableAnsi();
    printf("%s+===========================================+\n%s", colorTitle, colorReset);
    printf("%s|%s Welcome to %sSOSI.C%s Development Env %s|\n%s", colorTitle, colorReset, colorAccent, colorReset, colorTitle, colorReset);
    printf("%s+===========================================+\n%s", colorTitle, colorReset);
    printf("%s1%s. %sLaunch environment\n", colorKey, colorReset, colorText);
    printf("%s2%s. %sShow authors\n", colorKey, colorReset, colorText);
    printf("%s3%s. %sShow language syntax\n", colorKey, colorReset, colorText);
    printf("%s4%s. %sLoad file for execution\n", colorKey, colorReset, colorText);
    printf("%s5%s. %sExit\n", colorKey, colorReset, colorText);
    printf("%s(Press ESC to exit any time)%s\n", colorAccent, colorReset);
    printf("%sChoose an option: %s", colorKey, colorReset);
}

void showauthors()
{
    printf("%sAuthors:%s\n", colorAccent, colorReset);
    printf(" - amiloid\n");
    printf(" - faeris\n");
    printf(" - Marina\n");
    printf("\nPress any key to return to menu...");
}

void showsyntax()
{
    printf("%sSyntax and features:%s\n", colorAccent, colorReset);
    printf(" - set x 10\n");
    printf(" - arr myarray 1,2,3\n");
    printf(" - adarr myarray 4\n");
    printf(" - suarr myarray 2\n");
    printf(" - print x\n");
    printf(" - print \"text\"\n");
    printf(" - print myarray\n");
    printf(" - crun\n");
    printf(" - sloop N ... eloop\n");
    printf(" - savef filename.sosi\n");
    printf(" - loadf filename.sosi\n");
    printf("\nPress any key to return to menu...");
}

void savecode(const char* filename)
{
    if (!filename || filename[0] == '\0') {
        printf("Error: filename empty\n");
        return;
    }
    FILE* f = fopen(filename, "w");
    if (!f) {
        printf("Error: cannot open file %s\n", filename);
        return;
    }
    for (int i = 0; i < codeLines; i++) {
        fprintf(f, "%s\n", codeBuffer[i]);
    }
    fclose(f);
    printf("%sSaved to %s%s\n", colorInfo, filename, colorReset);
}

void loadcode(const char* filename)
{
    if (!filename || filename[0] == '\0') {
        printf("Error: filename empty\n");
        return;
    }
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Error: cannot open file %s\n", filename);
        return;
    }
    codeLines = 0;
    while (fgets(codeBuffer[codeLines], sizeof(codeBuffer[0]), f)) {
        codeBuffer[codeLines][strcspn(codeBuffer[codeLines], "\n")] = '\0';
        codeLines++;
        if (codeLines >= MAXLINES) break;
    }
    fclose(f);
    printf("%sLoaded from %s%s\n", colorInfo, filename, colorReset);
}

void printcoloredline(const char* line)
{
    if (!line) return;

    const char* keywords[] = {
        "set", "arr", "adarr", "suarr", "print", "crun", "savef", "loadf", "sloop", "eloop"
    };
    int nkeys = sizeof(keywords) / sizeof(keywords[0]);

    char copy[256];
    strncpy(copy, line, sizeof(copy) - 1);
    copy[sizeof(copy) - 1] = '\0';

    char* tok = strtok(copy, " ");
    int first = 1;
    while (tok) {
        int isk = 0;
        int isnum = 1;
        int isstr = (tok[0] == '"');

        for (int i = 0; i < nkeys; i++) if (strcmp(tok, keywords[i]) == 0) { isk = 1; break; }
        for (int i = 0; tok[i]; i++) if (!(tok[i] >= '0' && tok[i] <= '9')) { isnum = 0; break; }

        if (!first) printf(" ");

        if (isk) printf("%s%s%s", colorAccent, tok, colorReset);
        else if (isstr) printf("%s%s%s", colorKey, tok, colorReset);
        else if (isnum) printf("%s%s%s", colorInfo, tok, colorReset);
        else printf("%s%s%s", colorText, tok, colorReset);

        tok = strtok(NULL, " ");
        first = 0;
    }
    printf("\n");
}

void rundev()
{
    loading("Launching environment");
    printf("%sDevelopment environment started.%s Type code lines below. Use 'crun' to execute.\n", colorInfo, colorReset);

    int current = 0;
    char input[MAXINPUT];

    while (1) {
        printf("%d.   ", current + 1);
        fflush(stdout);

        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "crun") == 0) {
            loading("Executing code");
            execcode();

            printf("\nWhat to do next?\n");
            printf("1. Exit to menu\n");
            printf("2. Save code\n");
            printf("Choose: ");
            int action = 0;
            if (scanf("%d", &action) != 1) {
                while (getchar() != '\n');
                action = 1;
            }
            getchar(); 

            if (action == 2) {
                char fname[256];
                printf("Enter save command (savef filename.sosi): ");
                if (fgets(fname, sizeof(fname), stdin)) {
                    fname[strcspn(fname, "\n")] = '\0';
                    if (strncmp(fname, "savef ", 6) == 0) execcmd(fname);
                    else {
                        char buf[300];
                        snprintf(buf, sizeof(buf), "savef %s", fname);
                        execcmd(buf);
                    }
                }
            }

            break;
        } else {
            if (current < MAXLINES) {
                strncpy(codeBuffer[current], input, sizeof(codeBuffer[0]) - 1);
                codeBuffer[current][sizeof(codeBuffer[0]) - 1] = '\0';
                if (current == codeLines) codeLines++;
                printf("\033[F\033[2K");
                printf("%d.   ", current + 1);
                printcoloredline(input);
                current++;
            } else {
                printf("Code buffer is full!\n");
            }
        }
    }
}
