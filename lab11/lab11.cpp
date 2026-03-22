#define _CRT_SECURE_NO_WARNINGS   // Разрешает fopen, scanf, fscanf
#define _CRT_NONSTDC_NO_DEPRECATE // Разрешает read, write
#define _CRT_SECURE_NO_DEPRECATE

#include <iostream>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <io.h>

using namespace std;

// Исправляем ошибку "идентификатор gets не определен" для Visual Studio
#define gets(str) fgets(str, sizeof(str), stdin); str[strcspn(str, "\n")] = 0

int main() {
    setlocale(LC_ALL, "Russian");

    // --- ЧАСТЬ 1: ДЕМОНСТРАЦИЯ ФУНКЦИЙ (ЗАДАНИЕ I.1) ---
    printf("=== Part 1: Demo Functions ===\n");

    // 1. cin / cout
    int demoNum = 0;
    cout << "Enter number (cin): ";
    cin >> demoNum;
    cout << "Value: " << demoNum << endl;

    // 2. scanf / printf
    char demoName[100] = { 0 };
    printf("Enter name (scanf): ");
    scanf("%s", demoName);
    printf("Hello, %s!\n", demoName);

    // 3. gets / puts (теперь работает через макрос)
    char demoLine[100] = { 0 };
    getchar(); // Очистка буфера после scanf
    printf("Enter string (gets): ");
    gets(demoLine);
    puts("Result (puts):");
    puts(demoLine);

    // 4. read / write 
    char lowBuffer[100] = { 0 };
    printf("Enter 3 chars (read): ");
    int b = read(0, lowBuffer, 3);
    write(1, "Write: ", 7);
    write(1, lowBuffer, b);
    printf("\n\n");


    // --- ЧАСТЬ 2: КОРНЕВЫЕ СЛОВА (ЗАДАНИЕ I.2) ---
    printf("=== Part 2: Root Words (Task II.2) ===\n");

    char words[50][26] = { 0 }; // 50 слов до 25 символов
    int counts[50] = { 0 };
    int n = 0;

    // Чтение из файла (fopen, fscanf, feof, EOF, fclose)
    FILE* fIn = fopen("input.txt", "r");
    if (!fIn) {
        printf("Error: input.txt not found! Create it in the project folder.\n");
    }
    else {
        while (!feof(fIn) && n < 50) {
            if (fscanf(fIn, "%25s", words[n]) != EOF) {
                n++;
            }
        }
        fclose(fIn);
    }

    // Алгоритм поиска корневых слов
    bool hasRoots = false;
    for (int i = 0; i < n; i++) {
        int lenI = (int)strlen(words[i]);
        if (lenI == 0) continue;
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            // Проверка, является ли words[i] началом words[j]
            if (strncmp(words[i], words[j], lenI) == 0) {
                counts[i]++;
                hasRoots = true;
            }
        }
    }

    // Запись в файлы (fprintf)
    FILE* fOut1 = fopen("res1.txt", "w");
    FILE* fOut2 = fopen("res2.txt", "w");

    if (fOut1 && fOut2) {
        if (!hasRoots) {
            fprintf(fOut1, "NO");
            fprintf(fOut2, "NO");
        }
        else {
            int longIdx = -1, importIdx = -1;

            for (int i = 0; i < n; i++) {
                if (counts[i] > 0) {
                    if (longIdx == -1 || strlen(words[i]) > strlen(words[longIdx]))
                        longIdx = i;
                    if (importIdx == -1 || counts[i] > counts[importIdx])
                        importIdx = i;
                }
            }

            // Вывод в res1.txt
            fprintf(fOut1, "%s\n", words[longIdx]);
            int lenLong = (int)strlen(words[longIdx]);
            for (int j = 0; j < n; j++) {
                if (longIdx != j && strncmp(words[longIdx], words[j], lenLong) == 0) {
                    fprintf(fOut1, "%s ", words[j]);
                }
            }

            // Вывод в res2.txt
            fprintf(fOut2, "%s %d", words[importIdx], counts[importIdx]);
        }
        fclose(fOut1);
        fclose(fOut2);
    }

    printf("Done! Check res1.txt and res2.txt\n");
    return 0;
}
