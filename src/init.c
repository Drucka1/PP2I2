#include "../include/init.h"

int numLevelFromChar(char* filename){
    int level;
    sscanf(filename,"assets/level%d.txt",&level);
    return level;
}

Map* FileToMap(char *nomFichier,int pos_x, int pos_y, SDL_Texture** textures){
    Map* map = malloc(sizeof(Map));
    SDL_Texture* wallSprite = textures[WALL];
    SDL_Texture* groundSprite = textures[GROUND];
    SDL_Texture* doorSprite = textures[DOOR];
    SDL_Texture* keySprite = textures[KEY];
    SDL_Texture* leverSprite = textures[LEVER];

    int level = numLevelFromChar(nomFichier);
    map->level = level;

    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        perror("Erreur d'ouverture du fichier");
        return NULL;
    }

    int rows,cols,p,q;
    fscanf(fichier, "%d %d", &rows, &cols);
    map->cols = cols;
    map->rows = rows;
    int i = 0; int j = 0;
    char buffer[100];
    Cell** grid = malloc(sizeof(Cell*)*rows);
    for (int i = 0;i<rows;i++){
        grid[i] = malloc(sizeof(Cell)*cols);
        for (int j = 0;j<cols;j++) {
            grid[i][j].steppable = true;
            grid[i][j].numberObjects = 0;
            grid[i][j].objects = NULL;
            grid[i][j].map_tp = NULL;
        }
    }

    while (fscanf(fichier, "%s", buffer) != EOF) {
        if(strcmp(buffer,"/")==0){
            i++;
            j=0;
        }
        else if(strcmp(buffer,"|")==0){
            j++;         
        }
        else if (atoi(buffer) == 0 && strlen(buffer) > 10){
            grid[i][j].map_tp = malloc(sizeof(char)*(strlen(buffer)+1));
            strcpy(grid[i][j].map_tp,buffer);
        } 
        else if (atoi(buffer) == WALL){
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
            wall->door = NULL;
            grid[i][j].objects = listObjAppend(grid[i][j].objects,wall);
        } 
        else if (atoi(buffer) == GROUND){
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
            ground->door = NULL;
            grid[i][j].objects = listObjAppend(grid[i][j].objects,ground);
        }
        else if (atoi(buffer) == DOOR){
            grid[i][j].steppable &= false;
            grid[i][j].numberObjects++;

            SDL_Rect* pos = malloc(sizeof(SDL_Rect));
            pos->x = j*SIZE_WALL_W+pos_x;
            pos->y = i*SIZE_WALL_H+pos_y;
            pos->w = SIZE_WALL_W;
            pos->h = SIZE_WALL_H;

            Object* door = malloc(sizeof(Object));
            door->type_object = DOOR;
            door->texture = doorSprite;
            door->pos = pos;
            door->door = NULL;
            grid[i][j].objects = listObjAppend(grid[i][j].objects,door);
        }
        else if (sscanf(buffer,"4(%d,%d)",&p,&q) == 2){
            grid[i][j].steppable &= true;
            grid[i][j].numberObjects++;

            SDL_Rect* pos = malloc(sizeof(SDL_Rect));
            pos->x = j*SIZE_WALL_W+pos_x;
            pos->y = i*SIZE_WALL_H+pos_y;
            pos->w = SIZE_WALL_W;
            pos->h = SIZE_WALL_H;

            Object* lever = malloc(sizeof(Object));
            lever->type_object = LEVER;
            lever->texture = leverSprite;
            lever->pos = pos;
            
            lever->door = malloc(sizeof(Triple));
            lever->door->level = level;
            lever->door->x = p;
            lever->door->y = q;

            grid[i][j].objects = listObjAppend(grid[i][j].objects,lever);
        }
        else if (sscanf(buffer,"3(%d,%d)",&p,&q) == 2){
            grid[i][j].steppable &= true;
            grid[i][j].numberObjects++;

            SDL_Rect* pos = malloc(sizeof(SDL_Rect));
            pos->x = j*SIZE_WALL_W+pos_x;
            pos->y = i*SIZE_WALL_H+pos_y;
            pos->w = SIZE_WALL_W;
            pos->h = SIZE_WALL_H;

            Object* key = malloc(sizeof(Object));
            key->type_object = KEY;
            key->texture = keySprite;
            key->pos = pos;

            key->door = malloc(sizeof(Triple));
            key->door->level = level;
            key->door->x = p;
            key->door->y = q;

            grid[i][j].objects = listObjAppend(grid[i][j].objects,key);
        }
    }
    fclose(fichier);
    map->grid = grid;
    return map;
}

void dimMap(char* nomFichier, int* rows, int* cols){
    FILE *fichier = fopen(nomFichier, "r");
    fscanf(fichier, "%d %d", rows, cols);
    fclose(fichier);
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