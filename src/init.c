#include "../include/init.h"
#include "../include/aux.h"

int** FileToMap(char *nomFichier, int* rows, int* cols) {
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

SDL_Rect* init_terrain(int** terrain,int rows,int cols, int pos_x, int pos_y){
    SDL_Rect* mur_terrain = malloc(sizeof(SDL_Rect)*sommeMatrice(terrain,rows,cols));
    int k = 0;
    for (int i = 0;i<rows;i++){
        for (int j = 0;j<cols;j++){
            if (terrain[i][j] == 1){
                mur_terrain[k].x = j*SIZE_WALL_W+pos_x;
                mur_terrain[k].y = i*SIZE_WALL_H+pos_y;
                mur_terrain[k].w = SIZE_WALL_W;
                mur_terrain[k].h = SIZE_WALL_H;
                k++;
            }
        }
    }
    return mur_terrain;
}

void free_terrain(int** terrain,int rows){
    for(int i = 0;i<rows;i++){
        free(terrain[i]);
    }
    free(terrain);
}