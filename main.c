#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "solver.h"

void printGrid(int grid[9][9]) {
printf("---------------------\n");
for (int i = 0; i < 9; i++) {
for (int j = 0; j < 9; j++) {
if (j % 3 == 0 && j != 0) {
printf("| ");
}
if (grid[i][j] == 0) {
printf(". ");
} else {
printf("%d ", grid[i][j]);
}
}
printf("\n");
if ((i + 1) % 3 == 0 && i < 8) {
printf("---------------------\n");
}
}
printf("---------------------\n");
}

bool loadPuzzleFromFile(const char *filename, int grid[9][9]) {
FILE *file = fopen(filename, "r");
if (file == NULL) {
return false;
}

for (int i = 0; i < 9; i++) {
for (int j = 0; j < 9; j++) {
if (fscanf(file, "%d", &grid[i][j]) != 1) {
char temp[20];
if (fscanf(file, "%s", temp) == 1) {
fclose(file);
return true;
}
fclose(file);
return false;
}
}
}
fclose(file);
return true;
}

void savePuzzleToFile(const char *filename, int unsolvedGrid[9][9], int solvedGrid[9][9]) {
FILE *file = fopen(filename, "w");
if (file == NULL) {
printf("Error: Could not open file '%s' for writing.\n", filename);
return;
}

fprintf(file, "--- UNSOLVED PUZZLE ---\n");
for (int i = 0; i < 9; i++) {
for (int j = 0; j < 9; j++) {
fprintf(file, "%d ", unsolvedGrid[i][j]);
}
fprintf(file, "\n");
}

fprintf(file, "\n--- SOLVED PUZZLE ---\n");
for (int i = 0; i < 9; i++) {
for (int j = 0; j < 9; j++) {
fprintf(file, "%d ", solvedGrid[i][j]);
}
fprintf(file, "\n");
}

fclose(file);
printf("Solved puzzle (with original) saved to %s.\n", filename);
}

void enterPuzzleManually(int grid[9][9]) {
printf("Enter the puzzle (81 digits, 0 for empty), row by row:\n");
for (int i = 0; i < 9; i++) {
printf("Row %d: ", i + 1);
for (int j = 0; j < 9; j++) {
while (scanf("%1d", &grid[i][j]) != 1) {
printf("Invalid input. Please enter numbers only.\n");
while (getchar() != '\n');
printf("Row %d, position %d: ", i + 1, j + 1);
}
}
while (getchar() != '\n');
}
}

void displayMenu() {
printf("\n==== Sudoku Solver Menu ====\n");
printf("1. Enter Sudoku manually\n");
printf("2. Load Sudoku from file\n");
printf("3. Solve Sudoku\n");
printf("4. Display current Sudoku\n");
printf("5. Save solved Sudoku to file\n");
printf("6. Clear screen\n");
printf("7. Reset to original puzzle\n");
printf("0. Exit\n");
printf("Choose an option: ");
}

int main() {
int grid[9][9] = {0};
int unsolvedGrid[9][9] = {0};
char filename[100];
int choice;
bool puzzleLoaded = false;
bool puzzleSolved = false;

while (true) {
displayMenu();
if (scanf("%d", &choice) != 1) {
printf("Invalid input. Please enter a number.\n");
while (getchar() != '\n');
continue;
}

switch (choice) {
case 1:
enterPuzzleManually(grid);
memcpy(unsolvedGrid, grid, sizeof(int) * 81);
puzzleLoaded = true;
puzzleSolved = false;
printf("Puzzle entered. Here is your grid:\n");
printGrid(grid);
break;

case 2:
printf("Enter filename: ");
scanf("%99s", filename);
if (loadPuzzleFromFile(filename, grid)) {
memcpy(unsolvedGrid, grid, sizeof(int) * 81);
puzzleLoaded = true;
puzzleSolved = false;
printf("Puzzle loaded from %s:\n", filename);
printGrid(grid);
} else {
printf("Error: Could not load file '%s'.\n", filename);
}
break;

case 3:
if (!puzzleLoaded) {
printf("Error: No puzzle loaded.\n");
} else if (puzzleSolved) {
printf("This puzzle is already solved!\n");
} else {
memcpy(grid, unsolvedGrid, sizeof(int) * 81);
printf("Solving...\n");
if (solveSudoku(grid)) {
puzzleSolved = true;
printf("Puzzle solved!\n");
printGrid(grid);
} else {
printf("No solution found.\n");
}
}
break;

case 4:
if (!puzzleLoaded) {
printf("Error: No puzzle loaded.\n");
} else {
printf("\nCurrent Grid:\n");
printGrid(grid);
}
break;

case 5:
if (!puzzleSolved) {
printf("Error: Puzzle is not solved yet.\n");
} else {
printf("Enter filename to save to: ");
scanf("%99s", filename);
savePuzzleToFile(filename, unsolvedGrid, grid);
}
break;

case 6:
#ifdef _WIN32
system("cls");
#else
system("clear");
#endif
break;

case 7:
if (!puzzleLoaded) {
printf("Error: No puzzle loaded to reset.\n");
} else {
memcpy(grid, unsolvedGrid, sizeof(int) * 81);
puzzleSolved = false;
printf("Puzzle has been reset to its original state:\n");
printGrid(grid);
}
break;

case 0:
printf("Thank you for using the solver. Goodbye!\n");
return 0;

default:
printf("Invalid option. Please choose from 0-7.\n");
}

if (choice != 0 && choice != 6) {
printf("\nPress Enter to continue...");
while (getchar() != '\n');
getchar();
}
}
}
