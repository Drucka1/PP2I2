#include "../include/init.h"
#include "../include/aux.h"
#include "../include/struct.h"

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

wall* init_terrain(int** terrain,int rows,int cols, int pos_x, int pos_y,SDL_Texture* sprite){
    wall* murs = malloc(sizeof(wall)*sommeMatrice(terrain,rows,cols));
    int k = 0;
    for (int i = 0;i<rows;i++){
        for (int j = 0;j<cols;j++){
            if (terrain[i][j]){
                murs[k].pos = malloc(sizeof(SDL_Rect));
                murs[k].pos->x = j*SIZE_WALL_W+pos_x;
                murs[k].pos->y = i*SIZE_WALL_H+pos_y;
                murs[k].pos->w = SIZE_WALL_W;
                murs[k].pos->h = SIZE_WALL_H;
                murs[k].sprite = sprite;
                k++;
            }
        }
    }
    return murs;
}

void free_walls(wall* murs, int nb_murs){
    for(int i = 0;i<nb_murs;i++){
        free(murs[i].pos);
    }
    free(murs);
}


void free_terrain(int** terrain,int rows){
    for(int i = 0;i<rows;i++){
        free(terrain[i]);
    }
    free(terrain);
}

SDL_Texture* load_sprite(SDL_Renderer* renderer, char chemin[]){
    SDL_Surface* spriteSurface = SDL_LoadBMP(chemin);
    if (!spriteSurface){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in sprite surface: %s", SDL_GetError());
        exit(-1);
    }

    SDL_Texture* spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
    if (!spriteSurface){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in sprite texture: %s", SDL_GetError());
        exit(-1);
    }
    SDL_FreeSurface(spriteSurface);
    return spriteTexture;
}