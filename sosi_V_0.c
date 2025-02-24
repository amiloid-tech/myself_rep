#define _CRT_SECURE_NO_WARNINGS // Отключаем предупреждения о небезопасных функциях

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_VARS 100
#define MAX_CODE_LINES 1000
#define MAX_INPUT_SIZE 256
#define MAX_ARRAY_SIZE 100

// Структура для хранения переменных
typedef struct {
    char name[32];
    int value;
    bool is_array;
    int array[MAX_ARRAY_SIZE];
    int array_size;
} Variable;

Variable variables[MAX_VARS];
int var_count = 0;

// Буфер для хранения кода
char code_buffer[MAX_CODE_LINES][256];
int code_lines = 0;

// Поиск переменной по имени
Variable* find_var(const char* name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return &variables[i];
        }
    }
    return NULL;
}

// Выполнение команды
void execute_command(const char* command) {
    char cmd[32], arg1[128], arg2[128];
    sscanf(command, "%s %s %s", cmd, arg1, arg2);

    if (strcmp(cmd, "set") == 0) {
        // Создание или обновление переменной
        Variable* var = find_var(arg1);
        if (!var) {
            var = &variables[var_count++];
            strcpy(var->name, arg1);
            var->is_array = false; // По умолчанию это не массив
        }
        var->value = atoi(arg2);
    } else if (strcmp(cmd, "arr") == 0) {
        // Создание массива
        Variable* var = find_var(arg1);
        if (!var) {
            var = &variables[var_count++];
            strcpy(var->name, arg1);
            var->is_array = true;
            var->array_size = 0;
        }

        // Разделение элементов массива
        char* token = strtok(arg2, ",");
        while (token != NULL && var->array_size < MAX_ARRAY_SIZE) {
            var->array[var->array_size++] = atoi(token);
            token = strtok(NULL, ",");
        }
    } else if (strcmp(cmd, "adarr") == 0) {
        // Добавление элемента в массив
        Variable* var = find_var(arg1);
        if (var && var->is_array && var->array_size < MAX_ARRAY_SIZE) {
            var->array[var->array_size++] = atoi(arg2);
        } else {
            printf("Error: Cannot add element to array %s\n", arg1);
        }
    } else if (strcmp(cmd, "suarr") == 0) {
        // Удаление элемента из массива
        Variable* var = find_var(arg1);
        if (var && var->is_array) {
            int value = atoi(arg2);
            for (int i = 0; i < var->array_size; i++) {
                if (var->array[i] == value) {
                    for (int j = i; j < var->array_size - 1; j++) {
                        var->array[j] = var->array[j + 1];
                    }
                    var->array_size--;
                    break;
                }
            }
        } else {
            printf("Error: Cannot remove element from array %s\n", arg1);
        }
    } else if (strcmp(cmd, "print") == 0) {
        if (arg1[0] == '"') {
            // Вывод текста
            printf("%s\n", arg1 + 1);
        } else {
            // Вывод переменной или массива
            Variable* var = find_var(arg1);
            if (var) {
                if (var->is_array) {
                    printf("[");
                    for (int i = 0; i < var->array_size; i++) {
                        printf("%d", var->array[i]);
                        if (i < var->array_size - 1) {
                            printf(", ");
                        }
                    }
                    printf("]\n");
                } else {
                    printf("%d\n", var->value);
                }
            } else {
                printf("Unknown variable: %s\n", arg1);
            }
        }
    } else if (strcmp(cmd, "crun") == 0) {
        // Ничего не делаем, так как "crun" обрабатывается отдельно
    } else if (strcmp(cmd, "savef") == 0) {
        // Сохранение кода в файл
        FILE* file = fopen(arg1, "w");
        if (!file) {
            printf("Error: Could not open file %s\n", arg1);
            return;
        }
        for (int i = 0; i < code_lines; i++) {
            fprintf(file, "%s\n", code_buffer[i]);
        }
        fclose(file);
        printf("Code saved to %s\n", arg1);
    } else if (strcmp(cmd, "loadf") == 0) {
        // Загрузка кода из файла
        FILE* file = fopen(arg1, "r");
        if (!file) {
            printf("Error: Could not open file %s\n", arg1);
            return;
        }
        code_lines = 0;
        while (fgets(code_buffer[code_lines], sizeof(code_buffer[0]), file)) {
            code_buffer[code_lines][strcspn(code_buffer[code_lines], "\n")] = '\0';
            code_lines++;
        }
        fclose(file);
        printf("Code loaded from %s\n", arg1);
    } else {
        printf("Unknown command: %s\n", cmd);
    }
}

// Чтение и выполнение кода из буфера
void execute_code() {
    for (int i = 0; i < code_lines; i++) {
        execute_command(code_buffer[i]);
    }
}

// Вывод меню
void print_menu() {
    printf("Welcome to SOSI.C Development Environment!\n");
    printf("1. Launch Development Environment\n");
    printf("2. Show Authors\n");
    printf("3. Show Language Syntax and Features\n");
    printf("4. Load File for Execution\n");
    printf("Choose an option: ");
}

// Вывод авторов
void show_authors() {
    printf("Authors:\n");
    printf("- amiloid\n");
    printf("- faeris\n");
}

// Вывод синтаксиса и возможностей языка
void show_syntax() {
    printf("SOSI.C Language Syntax and Features:\n");
    printf("- set x 10: Set variable x to 10\n");
    printf("- arr my_array 1, 2, 3: Create array with elements 1, 2, 3\n");
    printf("- adarr my_array 4: Add element 4 to array\n");
    printf("- suarr my_array 2: Remove element 2 from array\n");
    printf("- print x: Print value of variable x\n");
    printf("- print \"text\": Print text\n");
    printf("- print my_array: Print all elements of array\n");
    printf("- crun: Execute the code\n");
    printf("- savef filename.sosi: Save code to file\n");
    printf("- loadf filename.sosi: Load code from file\n");
}

// Основная функция
int main() {
    int choice;
    print_menu();
    scanf("%d", &choice);
    getchar(); // Убираем символ новой строки после ввода числа

    switch (choice) {
        case 1:
            // Запуск среды разработки
            break;
        case 2:
            show_authors();
            return 0;
        case 3:
            show_syntax();
            return 0;
        case 4: {
            char filename[256];
            printf("Enter file path (e.g., loadf filename.sosi): ");
            scanf("%s", filename);
            execute_command(filename); // Загружаем файл
            break;
        }
        default:
            printf("Invalid choice.\n");
            return 1;
    }

    printf("Development Environment Started. Type your code below. Use 'crun' to execute the code.\n");

    int current_line = 0;
    char input[MAX_INPUT_SIZE];
    while (1) {
        // Вывод номера строки
        printf("%d.   ", current_line + 1);

        // Чтение ввода
        if (fgets(input, sizeof(input), stdin)) {
            input[strcspn(input, "\n")] = '\0'; // Убираем символ новой строки

            if (strcmp(input, "crun") == 0) {
                // Выполнение кода
                printf("\nExecuting code...\n");
                execute_code();
                printf("\nWhat to do next?\n");
                printf("1. Exit\n");
                printf("2. Save code\n");
                printf("Choose an option: ");
                int action;
                scanf("%d", &action);
                getchar(); // Убираем символ новой строки

                if (action == 2) {
                    char filename[256];
                    printf("Enter save command (e.g., savef filename.sosi): ");
                    scanf("%s", filename);
                    execute_command(filename); // Сохраняем код
                }
                break;
            } else {
                // Сохранение строки в буфер
                if (current_line < MAX_CODE_LINES) {
                    strcpy(code_buffer[current_line], input);
                    if (current_line == code_lines) {
                        code_lines++;
                    }
                    current_line++;
                } else {
                    printf("Code buffer is full!\n");
                }
            }
        }
    }

    return 0;
}