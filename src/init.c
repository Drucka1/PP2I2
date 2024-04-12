#include "../include/init.h"
#include "../include/aux.h"
#include "../include/struct.h"
#include "../include/list.h"
#include <SDL2/SDL_image.h>

List* ** FileToMap(char *nomFichier, int* rows, int* cols) {
    FILE *fichier = fopen(nomFichier, "r");

    fscanf(fichier, "%d %d", rows, cols);

    List* **matrice = malloc(sizeof(List**)*(*rows));
    for (int i = 0; i < *rows; i++) {
        matrice[i] = malloc(sizeof(List*)*(*cols));
        for (int j = 0; j < *cols; j++) {
            matrice[i][j] = NULL;
        }
    }
    
    int p = 0;int q = 0;
    char tmp[5];
    while (fscanf(fichier, "%s", tmp) != EOF) {
        if(strcmp(tmp,"/")==0){
            p++;
            q=0;
        }
        else if(strcmp(tmp,"|")==0){
            q++;         
        }
        else {
            matrice[p][q] = listAppend(matrice[p][q],atoi(tmp));
        }
    }
    fclose(fichier);
    return matrice;
}

Map* initMap(List* **terrain,int rows,int cols, int pos_x, int pos_y, SDL_Texture** textures){
    Map* map = malloc(sizeof(Map));
    SDL_Texture* wallSprite = textures[WALL];
    SDL_Texture* groundSprite = textures[GROUND];
    SDL_Texture* doorSprite = textures[DOOR];
    SDL_Texture* keySprite = textures[KEY];

    Cell** grid = malloc(sizeof(Cell*)*rows);
    for (int i = 0;i<rows;i++){
        grid[i] = malloc(sizeof(Cell)*cols);
        for (int j = 0;j<cols;j++) {
            List* tmp = terrain[i][j]; 
            grid[i][j].steppable = true;
            grid[i][j].numberObjects = 0;
            ListObj* objects = NULL;
            while(tmp != NULL){
                if (tmp->x == WALL){
                    grid[i][j].steppable &= false;
                    grid[i][j].numberObjects++;
                   
                    SDL_Rect* pos = malloc(sizeof(SDL_Rect));
                    pos->x = j*SIZE_WALL_W+pos_x;
                    pos->y = i*SIZE_WALL_H+pos_y;
                    pos->w = SIZE_WALL_W;
                    pos->h = SIZE_WALL_H;

                    Object* wall = malloc(sizeof(Object));
                    wall->type_object = WALL;
                    wall->texture = wallSprite;
                    wall->pos = pos;

                    objects = listObjAppend(objects,wall);
                }
                if (tmp->x == GROUND){
                    grid[i][j].steppable &= true;
                    grid[i][j].numberObjects++;
                    
                    SDL_Rect* pos = malloc(sizeof(SDL_Rect));
                    pos->x = j*SIZE_WALL_W+pos_x;
                    pos->y = i*SIZE_WALL_H+pos_y;
                    pos->w = SIZE_WALL_W;
                    pos->h = SIZE_WALL_H;

                    Object* ground = malloc(sizeof(Object));
                    ground->type_object = GROUND;
                    ground->texture = groundSprite;
                    ground->pos = pos;

                    objects = listObjAppend(objects,ground);
                }
                if (tmp->x == DOOR){
                    grid[i][j].steppable &= false;
                    grid[i][j].numberObjects++;

                    SDL_Rect* pos = malloc(sizeof(SDL_Rect));
                    pos->x = j*SIZE_WALL_W+pos_x;
                    pos->y = i*SIZE_WALL_H+pos_y;
                    pos->w = SIZE_WALL_W;
                    pos->h = SIZE_WALL_H;

                    Object* ground = malloc(sizeof(Object));
                    ground->type_object = DOOR;
                    ground->texture = doorSprite;
                    ground->pos = pos;

                    objects = listObjAppend(objects,ground);
                }
                if (tmp->x == KEY){
                    grid[i][j].steppable &= true;
                    grid[i][j].numberObjects++;

                    SDL_Rect* pos = malloc(sizeof(SDL_Rect));
                    pos->x = j*SIZE_WALL_W+pos_x;
                    pos->y = i*SIZE_WALL_H+pos_y;
                    pos->w = SIZE_WALL_W;
                    pos->h = SIZE_WALL_H;

                    Object* key = malloc(sizeof(Object));
                    key->type_object = DOOR;
                    key->texture = keySprite;
                    key->pos = pos;

                    objects = listObjAppend(objects,key);
                }
                tmp = tmp->next;
            }
            grid[i][j].objects = objects;
        }
    }
    map->cols = cols;
    map->grid = grid;
    map->rows = rows;
    return map;
}


SDL_Texture* load_sprite(SDL_Renderer* renderer, char chemin[]){
    SDL_Surface* spriteSurface = IMG_Load(chemin);
    if (!spriteSurface) {
        printf( "Unable to load image %s ! SDL_image Error: %s\n", chemin, IMG_GetError() );
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