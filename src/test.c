#include <stdio.h>
#include <stdlib.h>

#define LIGNES 7
#define COLONNES 7

int** create_mat(int rows,int cols){
    int** mat = malloc(sizeof(int*)*rows);
    for (int i = 0; i < rows; i++) {
        mat[i] = malloc(sizeof(int)*cols);
        for (int j = 0; j < cols; j++) {
            if( i == 0 || i == rows-1 || j == 0 || j == cols-1 || rand() % 3 == 0){
                mat[i][j] = 1;
            }
            else { mat[i][j] = 0; }
        }
    }
    return mat;
}


void afficherMatrice(int** matrice, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%d ", matrice[i][j]);
        }
        printf("\n");
    }
}


int main() {
    afficherMatrice(create_mat(LIGNES,COLONNES),LIGNES,COLONNES);
}