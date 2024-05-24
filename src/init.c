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

SDL_Texture* createSubTexture(SDL_Texture* original, SDL_Rect* section) {
    SDL_Window *window;
    SDL_Renderer *renderer;
    initSDL(&window, &renderer);
    SDL_Texture* newTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, section->w, section->h);
    SDL_SetRenderTarget(renderer, newTexture);
    SDL_RenderCopy(renderer, original, section, NULL);
    SDL_SetRenderTarget(renderer, NULL);
    quitSDL(window, renderer);
    return newTexture;
}

/*
SDL_Texture* createSubTexture(SDL_Renderer* renderer, SDL_Texture* original, SDL_Rect* section) {
    Uint32 format;
    int access, w, h;
    SDL_QueryTexture(original, &format, &access, &w, &h);
    SDL_Texture* newTexture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_TARGET, section->w, section->h);
    if (newTexture == NULL) {
        // handle error
        return NULL;
    }

    // Set the new texture as the render target
    SDL_SetRenderTarget(renderer, newTexture);

    // Copy the section of the original texture to the new texture
    SDL_RenderCopy(renderer, original, section, NULL);

    // Reset the render target
    SDL_SetRenderTarget(renderer, NULL);
    return newTexture;
}

void setBlackPixels(SDL_Texture* texture, Uint8 r, Uint8 g, Uint8 b) {
    int pitch, width, height;
    void* pixels;

    // Récupérer la largeur et la hauteur de la texture
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    SDL_LockTexture(texture, NULL, &pixels, &pitch);

    Uint32* uPixels = (Uint32*)pixels;

    SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    Uint32 black = SDL_MapRGB(format, 0, 0, 0);
    Uint32 newColor = SDL_MapRGB(format, r, g, b);

    for (int i = 0; i < width * height; i++) {
        if (uPixels[i] == black) {
            uPixels[i] = newColor;
        }
    }

    SDL_UnlockTexture(texture);
    SDL_FreeFormat(format);
}*/

Map* FileToMap(char *nomFichier,SDL_Rect* posPlayer, SDL_Texture** textures){
    Map* map = malloc(sizeof(Map));

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
            else if (offset_player_x < map->cols-1 - NB_WALL_W/2) map->offset_map.x = -(offset_player_x - NB_WALL_W/2)*SIZE_WALL_W;
            else  map->offset_map.x = -(map->cols - NB_WALL_W)*SIZE_WALL_W;
        }
    }

    if (!map->offset_map.y){
        if(posPlayer) map->offset_map.y = MIN(0,-(offset_player_y - posPlayer->y / SIZE_WALL_H)*SIZE_WALL_H);
        else {
            if (offset_player_y < NB_WALL_H/2) map->offset_map.y = 0;
            else if (offset_player_y < map->rows-1 -NB_WALL_H/2) map->offset_map.y = -(offset_player_y - NB_WALL_H/2)*SIZE_WALL_H;
            else  map->offset_map.y = -(map->rows - NB_WALL_H )*SIZE_WALL_H;
        }
    }
    
    map->UpperLeftCorner.x = map->offset_map.x;
    map->UpperLeftCorner.y = map->offset_map.y;
    map->BottomRightCorner.x = (map->cols-1)*SIZE_WALL_W+map->offset_map.x;
    map->BottomRightCorner.y = (map->rows-1)*SIZE_WALL_H+map->offset_map.y;

    int p,q,num;
    char color;
    
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

        else if (sscanf(buffer,"n%d%c",&num,&color) == 2){
            grid[i][j].steppable &= true;
            grid[i][j].numberObjects++;

            SDL_Rect* pos = malloc(sizeof(SDL_Rect));
            pos->x = j*SIZE_WALL_W+map->offset_map.x;
            pos->y = i*SIZE_WALL_H+map->offset_map.y;
            pos->w = SIZE_WALL_W;
            pos->h = SIZE_WALL_H;

            Object* number = malloc(sizeof(Object));
            number->type_object = NUMBER;
            number->texture = textures[NUMBER];
            number->pos = pos;
            number->action = NULL;

            grid[i][j].objects = listObjAppend(grid[i][j].objects,number);
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
            lever->texture = textures[LEVER];
            lever->pos = pos;
            
            lever->action = malloc(sizeof(Triple));
            lever->action->level = level;
            lever->action->x = p;
            lever->action->y = q;

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
            key->texture = textures[KEY];
            key->pos = pos;

            key->action = malloc(sizeof(Triple));
            key->action->level = level;
            key->action->x = p;
            key->action->y = q;

            grid[i][j].objects = listObjAppend(grid[i][j].objects,key);
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
            wall->texture =textures[WALL];
            wall->pos = pos;
            wall->action = NULL;
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
            ground->texture = textures[GROUND];
            ground->pos = pos;
            ground->action = NULL;
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
            push->texture = textures[PUSH];
            push->pos = pos;
            push->action = NULL;
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
            ice->texture = textures[ICE];;
            ice->pos = pos;
            ice->action = NULL;
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
            door->texture = textures[DOOR];
            door->pos = pos;
            door->action = NULL;
            grid[i][j].objects = listObjAppend(grid[i][j].objects,door);
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
                if  (objs->object->action) fprintf(file, "(%d,%d)",objs->object->action->x,objs->object->action->y);
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
        obj->action = malloc(sizeof(Triple));
        obj->action->level = level;
        obj->action->x = x;
        obj->action->y = y;
        obj->pos =NULL;
        inventory = listObjAppend(inventory,obj);
    }

    fclose(fichier);

    Entity* player = malloc(sizeof(Entity));
    player->pos = pos;
    player->inventory = inventory;
    return player;
}