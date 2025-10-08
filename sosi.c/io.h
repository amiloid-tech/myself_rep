#ifndef IO_H
#define IO_H

#define MAXLINES 1000
#define MAXINPUT 256

void printmenu();
void showauthors();
void showsyntax();
void savecode(const char* filename);
void loadcode(const char* filename);
void rundev();
void printcoloredline(const char* line);

extern char codeBuffer[MAXLINES][256];
extern int codeLines;

int keyhit();
int keyget();

#endif
