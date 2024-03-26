#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 9
#define COLS 9
#define TRUE 1
#define FALSE 0

typedef struct Cell {
    int x;
    int y;
} Cell, cell;

int **readMatrix(char *fileName) {
    int **matrix = malloc(ROWS * sizeof(int *));

    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return matrix;
    }

    for (int i = 0; i < ROWS; i++) {
        matrix[i] = malloc(ROWS * sizeof(int));
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            fscanf(file, "%d ", &matrix[i][j]);
        }
    }

    fclose(file);

    return matrix;
}

void print(int **matrix) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}
/* Utility to print lines and crosses, used by print_matrix. */
void print_separator() {
    for (int i = 0; i < 3; ++i) {
        printf("+---------");
    }
    printf("+\n");
}

void print_matrix(int **matrix) {
    for (int i = 0; i < 9; ++i) {
        if ((i % 3) == 0) {
            print_separator();
        }
        for (int j = 0; j < 9; j++) {
            int cell = matrix[i][j];
            if ((j % 3) == 0) {
                printf("| ");
            } else {
                printf(" ");
            }
            printf("%d ", cell);
        }
        printf("|\n");
    }
    print_separator();
}

int **cloneMatrix(int **matrix) {
    int **res = malloc(ROWS * sizeof(int *));

    for (int i = 0; i < ROWS; i++) {
        res[i] = malloc(COLS * sizeof(int));
        for (int j = 0; j < COLS; j++) {
            res[i][j] = matrix[i][j];
        }
    }

    return res;
}

int isValidAt(int **matrix, Cell *cell, int guess) {
    if (!cell) {
        return FALSE;
    }
    int corner_x = cell->x / 3 * 3;
    int corner_y = cell->y / 3 * 3;

    for (int x = 0; x < 9; x++) {
        if (matrix[cell->x][x] == guess)
            return FALSE;
        if (matrix[x][cell->y] == guess)
            return FALSE;
        if (matrix[corner_x + (x % 3)][corner_y + (x / 3)] == guess)
            return FALSE;
    }

    return TRUE;
}

Cell *startAt(int **matrix) {

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (matrix[i][j] == 0) {
                Cell *res = malloc(sizeof(struct Cell));
                res->x = i;
                res->y = j;

                return res;
            }
        }
    }
    return NULL;
}

int backtracking(int **matrix) {
    Cell *start = startAt(matrix);
    if (start == NULL) {
        return 1;
    }

    for (int i = 1; i <= 9; i++) {
        if (isValidAt(matrix, start, i)) {
            matrix[start->x][start->y] = i;
            if (backtracking(matrix)) {
                return 1;
            }
            matrix[start->x][start->y] = 0;
        }
    }

    return 0;
}

int isSolution(int **matrix) {
    for (int x = 0; x < 9; x += 3) {
        for (int y = 0; y < 9; y += 3) {
            int count[10] = {0};
            for (int j = x; j < x + 3; j++) {
                for (int k = y; k < y + 3; k++) {
                    int cur = matrix[j][k];
                    if (cur > 0 && count[cur] > 0) {
                        return 0;
                    }
                    count[cur]++;
                }
            }
        }
    }

    return 1;
}

void useBacktracking(int **matrix) {
    printf("Backtracking solution:\n");
    backtracking(matrix);
    if (isSolution(matrix)) {
        print_matrix(matrix);
    }
}

int main(int argc, char *argv[]) {
    char *fileName = argv[1];
    int **matrix = readMatrix(fileName);

    print_matrix(matrix);
    int **clone = cloneMatrix(matrix);

    useBacktracking(clone);

    free(clone);
    free(matrix);
    return 1;
}
