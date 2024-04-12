
#include "../include/aux.h"

int sommeMatrice(int** matrice, int rows, int cols) {
    int somme = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            somme += matrice[i][j];
        }
    }
    return somme;
}

void freeTerrain(List* **terrain,int rows, int cols){
    for(int i = 0;i<rows;i++){
        for(int j = 0;j<cols;j++){
            freeList(terrain[i][j]);
        }
        free(terrain[i]);
    }
    free(terrain);
}

void freeMap(Map* map){
    for(int i = 0;i<map->rows;i++){
        for(int j = 0;j<map->cols;j++){
            freeListObj(map->grid[i][j].objects);
        }
        free(map->grid[i]);
    }
    free(map->grid);
    free(map);
}

void freeSprites(SDL_Texture** textures,int nbSprites){
    for(int i = 0;i<nbSprites;i++){
        SDL_DestroyTexture(textures[i]);
    }
    free(textures);
}

