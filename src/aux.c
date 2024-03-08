#include "../include/aux.h"
#include <stdlib.h>

int sommeMatrice(int** matrice, int rows, int cols) {
    int somme = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            somme += matrice[i][j];
        }
    }
    return somme;
}

void free_mat(int** matrice,int rows){
    for(int i = 0;i<rows;i++){
        free(matrice[i]);
    }
    free(matrice);
}