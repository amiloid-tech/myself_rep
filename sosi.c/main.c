#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "commands.h"


int main()
{
    int choice = 0;

    while (1) {
        system("cls||clear"); 
        printmenu();

        if (keyhit()) {
            int k = keyget();
            if (k == 27) { 
                printf("\nExiting program...\n");
                break;
            } else {
            }
        }

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Try again.\n");
            while (getchar() != '\n');
            continue;
        }
        getchar(); 

        switch (choice) {
            case 1:
                rundev();
                break;
            case 2:
                showauthors();
                keyget(); 
                break;
            case 3:
                showsyntax();
                keyget();
                break;
            case 4: {
                char fname[256];
                printf("Enter file path: ");
                if (scanf("%255s", fname) == 1) {
                    char cmdline[300];
                    snprintf(cmdline, sizeof(cmdline), "loadf %s", fname);
                    execcmd(cmdline);
                    printf("\nPress any key to return to menu...");
                    keyget();
                }
                break;
            }
            case 5:
                printf("Exiting program...\n");
                return 0;
            default:
                printf("Invalid choice.\n");
                printf("\nPress any key to return to menu...");
                keyget();
                break;
        }
    }

    return 0;
}
