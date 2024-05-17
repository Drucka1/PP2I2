#include "../include/init.h"

int numLevelFromChar(char* filename){
    int level = -1; // valeur par défaut
    char* lastSlash = strrchr(filename, '/');
    if (lastSlash != NULL) {
        sscanf(lastSlash, "/level%d.txt", &level);
    } else {
        sscanf(filename, "level%d.txt", &level);
    }

    if (level == -1) {
        printf("Erreur lors de la lecture du niveau à partir du nom du fichier\n");
    }

    return level;
}

Map* FileToMap(char *nomFichier,int pos_x, int pos_y, SDL_Texture** textures){
    Map* map = malloc(sizeof(Map));
    SDL_Texture* wallSprite = textures[WALL];
    SDL_Texture* groundSprite = textures[GROUND];
    SDL_Texture* doorSprite = textures[DOOR];
    SDL_Texture* keySprite = textures[KEY];
    SDL_Texture* leverSprite = textures[LEVER];
    SDL_Texture* iceSprite = textures[ICE];
    SDL_Texture* pushSprite = textures[PUSH];

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
        else if (atoi(buffer) == 0 && strlen(buffer) > 7){ //detection de tp IG
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
        else if (atoi(buffer) == PUSH){
            grid[i][j].steppable &= true;
            grid[i][j].numberObjects++;
            
            SDL_Rect* pos = malloc(sizeof(SDL_Rect)); 
            pos->x = j*SIZE_WALL_W+pos_x;
            pos->y = i*SIZE_WALL_H+pos_y;
            pos->w = SIZE_WALL_W;
            pos->h = SIZE_WALL_H;

            Object* push = malloc(sizeof(Object));
            push->type_object = PUSH;
            push->texture = pushSprite;
            push->pos = pos;
            push->door = NULL;
            grid[i][j].objects = listObjAppend(grid[i][j].objects,push);
        }
        else if(atoi(buffer)==ICE){ //ICI
            grid[i][j].steppable &= true;
            grid[i][j].numberObjects++;
            
            SDL_Rect* pos = malloc(sizeof(SDL_Rect));
            pos->x = j*SIZE_WALL_W+pos_x;
            pos->y = i*SIZE_WALL_H+pos_y;
            pos->w = SIZE_WALL_W;
            pos->h = SIZE_WALL_H;

            Object* ice = malloc(sizeof(Object));
            ice->type_object = ICE;
            ice->texture = iceSprite;
            ice->pos = pos;
            ice->door = NULL;
            grid[i][j].objects = listObjAppend(grid[i][j].objects,ice);
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


void MapToFile(Map* map) {
    char filename[70] = "assets/level/save/level";
    char lvl[10];
    sprintf(lvl, "%d", map->level);
    strcat(filename,lvl);
    strcat(filename,".txt");

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", filename);
        return;
    }

    fprintf(file, "%d %d\n", map->rows, map->cols);
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->cols; j++) {
            ListObj* objs = map->grid[i][j].objects;
            while (objs != NULL){
                fprintf(file, "%d",objs->object->type_object);
                if  (objs->object->door) fprintf(file, "(%d,%d)",objs->object->door->x,objs->object->door->y);
                fprintf(file, " ");
                objs = objs->next;
            }

            if (map->grid[i][j].map_tp) fprintf(file, "%s ", map->grid[i][j].map_tp);

            if (j == map->cols-1) fprintf(file, "/\n"); 
            else fprintf(file, "| ");
        }
    }
    fclose(file);
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

Entity* getPlayerStatus(int* current_level){
    char filename[70] = "assets/level/save/playerStatus.txt";
    FILE* fichier = fopen(filename, "r");

    fscanf(fichier, "%d\n", current_level);
    SDL_Rect* pos = malloc(sizeof(SDL_Rect));
    fscanf(fichier, "(%d,%d,%d,%d)\n",&pos->x,&pos->y,&pos->h,&pos->w);

    int type_obj,level,x,y;
    ListObj* inventory = NULL;

    while (fscanf(fichier, "%d(%d,%d,%d)\n", &type_obj,&level,&x,&y) != EOF){
        Object* obj = malloc(sizeof(Object));
        obj->type_object = type_obj;
        obj->door = malloc(sizeof(Triple));
        obj->door->level = level;
        obj->door->x = x;
        obj->door->y = y;
        obj->pos =NULL;
        inventory = listObjAppend(inventory,obj);
    }

    fclose(fichier);

    Entity* player = malloc(sizeof(Entity));
    player->pos = pos;
    player->inventory = inventory;
    return player;
}