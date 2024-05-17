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

void posInitPlayerLevel(char *nomFichier, int* offset_player_x, int* offset_player_y){
    FILE *fichier = fopen(nomFichier, "r");
    fscanf(fichier, "%d %d", offset_player_y, offset_player_x);
    fclose(fichier);
}

Map* FileToMap(char *nomFichier,SDL_Rect* posPlayer, SDL_Texture** textures){
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
    
    int offset_player_x,offset_player_y;
    fscanf(fichier,"%d %d\n",&offset_player_y,&offset_player_x);

    fscanf(fichier, "%d %d", &map->rows,&map->cols);
    map->offset_map.x = MAX(0,SIZE_WALL_W*(NB_WALL_W-map->cols)/2);
    map->offset_map.y = MAX(0,SIZE_WALL_H*(NB_WALL_H-map->rows)/2);

    if (!map->offset_map.x){
        if (posPlayer) map->offset_map.x = MIN(0,-(offset_player_x - posPlayer->x / SIZE_WALL_W)*SIZE_WALL_W);
        else {
            if (offset_player_x < NB_WALL_W/2) map->offset_map.x = 0;
            else if (offset_player_x < map->cols - NB_WALL_W/2) map->offset_map.x = -(offset_player_x - NB_WALL_W/2)*SIZE_WALL_W;
            else  map->offset_map.x = -(map->cols - NB_WALL_W)*SIZE_WALL_W;
        }

    }

    if (!map->offset_map.y){
        if(posPlayer) map->offset_map.y = MIN(0,-(offset_player_y - posPlayer->y / SIZE_WALL_H)*SIZE_WALL_H);
        else {
            if (offset_player_y < NB_WALL_H/2) map->offset_map.y = 0;
            else if (offset_player_y < map->rows -NB_WALL_H/2) map->offset_map.y = -(offset_player_y - NB_WALL_H/2)*SIZE_WALL_H;
            else  map->offset_map.y = -(map->rows - NB_WALL_H )*SIZE_WALL_H;
        }
    }
    
    map->UpperLeftCorner.x = map->offset_map.x;
    map->UpperLeftCorner.y = map->offset_map.y;
    map->BottomRightCorner.x = (map->cols-1)*SIZE_WALL_W+map->offset_map.x;
    map->BottomRightCorner.y = (map->rows-1)*SIZE_WALL_H+map->offset_map.y;

    int p,q;
    
    char buffer[100];
    Cell** grid = malloc(sizeof(Cell*)*map->rows);
    for (int i = 0;i<map->rows;i++){
        grid[i] = malloc(sizeof(Cell)*map->cols);
        for (int j = 0;j<map->cols;j++) {
            grid[i][j].steppable = true;
            grid[i][j].numberObjects = 0;
            grid[i][j].objects = NULL;
            grid[i][j].map_tp = NULL;
        }
    }
    
    int i = 0; int j = 0;
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
            pos->x = j*SIZE_WALL_W+map->offset_map.x;
            pos->y = i*SIZE_WALL_H+map->offset_map.y;
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
            pos->x = j*SIZE_WALL_W+map->offset_map.x;
            pos->y = i*SIZE_WALL_H+map->offset_map.y;
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
            pos->x = j*SIZE_WALL_W+map->offset_map.x;
            pos->y = i*SIZE_WALL_H+map->offset_map.y;
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
            pos->x = j*SIZE_WALL_W+map->offset_map.x;
            pos->y = i*SIZE_WALL_H+map->offset_map.y;
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
            pos->x = j*SIZE_WALL_W+map->offset_map.x;
            pos->y = i*SIZE_WALL_H+map->offset_map.y;
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
            pos->x = j*SIZE_WALL_W+map->offset_map.x;
            pos->y = i*SIZE_WALL_H+map->offset_map.y;
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
            pos->x = j*SIZE_WALL_W+map->offset_map.x;
            pos->y = i*SIZE_WALL_H+map->offset_map.y;
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


void MapToFile(Map* map,int posMapX, int posMapY) {
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
    fprintf(file, "%d %d\n",posMapY,posMapX);
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
    fscanf(fichier, "%*[^\n]\n");
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

void tp(Map* *map,int i, int j,Entity* player,SDL_Texture** textures){
    
    MapToFile(*map,(player->pos->x - (*map)->grid[0][0].objects->object->pos->x) / SIZE_WALL_W,(player->pos->y - (*map)->grid[0][0].objects->object->pos->y) / SIZE_WALL_W);

    char* lvl = (*map)->grid[i][j].map_tp;
    char path_level[100] = "assets/level/save/";
    strcat(path_level,lvl);
    if(access(path_level, F_OK) != 0) {
        strcpy(path_level, "assets/level/default/");
        strcat(path_level,lvl);
    }

    freeMap(*map);
    int offset_player_x,offset_player_y;
    posInitPlayerLevel(path_level,&offset_player_x,&offset_player_y);
    *map = FileToMap(path_level,NULL,textures);

    player->pos->x = (*map)->offset_map.x+offset_player_x*SIZE_WALL_W;
    player->pos->y = (*map)->offset_map.y+offset_player_y*SIZE_WALL_H; 
}