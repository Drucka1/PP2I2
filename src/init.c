#include "../include/init.h"
#include "../include/aux.h"

int** FileToMap(const char *nomFichier, int* rows, int* cols) {
    FILE *fichier = fopen(nomFichier, "r");

    fscanf(fichier, "%d %d", rows, cols);

    int** matrice = malloc(sizeof(int*)*(*rows));
    for (int i = 0; i < *rows; i++) {
        matrice[i] = malloc(sizeof(int)*(*cols));
        for (int j = 0; j < *cols; j++) {
            fscanf(fichier, "%d ", &matrice[i][j]);
            
        }
    }
    fclose(fichier);
    return matrice;
}

SDL_Rect* init_terrain(int** terrain,int rows,int cols){
    SDL_Rect* mur_terrain = malloc(sizeof(SDL_Rect)*sommeMatrice(terrain,rows,cols));
    int k = 0;
    for (int i = 0;i<rows;i++){
        for (int j = 0;j<cols;j++){
            if (terrain[i][j] == 1){
                mur_terrain[k].x = j*SIZE_WALL_W;
                mur_terrain[k].y = i*SIZE_WALL_H;
                mur_terrain[k].w = SIZE_WALL_W;
                mur_terrain[k].h = SIZE_WALL_H;
                k++;
            }
        }
    }
    return mur_terrain;
}