#include "../include/game.h"

int main(int argc, char* argv[]){

    if(argc < 2){
        argv[1] = "assets/level1.txt";
    }
    
    SDL_Window *window;
    SDL_Renderer *renderer;
    initSDL(&window, &renderer);

    //Terrain
    int cols,rows;
    dimMap(argv[1],&rows,&cols);
    int pos_x = MAX(0,SIZE_WALL_W*(NB_WALL_W-cols)/2);
    int pos_y = MAX(0,SIZE_WALL_H*(NB_WALL_H-rows)/2);
    
    SDL_Texture** textures = malloc(sizeof(SDL_Texture*)*NB_SPRITES);
    textures[WALL] = load_sprite(renderer,"assets/wall.png");
    textures[GROUND] = load_sprite(renderer,"assets/ground.png");
    textures[DOOR] = load_sprite(renderer,"assets/door2.png");
    textures[KEY] = load_sprite(renderer,"assets/key.png");
    textures[LEVER] = load_sprite(renderer,"assets/lever.png");

    Map** maps = malloc(sizeof(Map*)*NB_LEVEL);
    for(int i = 1;i<NB_LEVEL;i++){
        maps[i] = NULL;
    }

    maps[0] = FileToMap(argv[1],pos_x,pos_y,textures);
    
    Map* map = maps[0];
    Tuple wall0 = {0,0};
    Tuple wallf = {(cols-1)*SIZE_WALL_W,(rows-1)*SIZE_WALL_H};
    
    //Player
    SDL_Texture* spriteTexture = load_sprite(renderer, "./assets/player.png");
    Entity player;
    player.pos = &(SDL_Rect){SIZE_WALL_W+pos_x,SIZE_WALL_H+pos_y,SIZE_WALL_W,SIZE_WALL_H};
    player.texture = spriteTexture;
    player.inventory = NULL;

    int spriteFullWIdth,spriteFullHeight;
    if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWIdth, &spriteFullHeight)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in query texture: %s", SDL_GetError());
        exit(-1);
    }

    int spriteWidth = spriteFullWIdth/4;
    int spriteHeight = spriteFullHeight/4;
    SDL_Event event;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    int running=1;int offset = 0;int direction = 0;
    int dark = !true;
    
    while (running) { 
        //Verfie si le joueur peut changer de niveau/se tp
        bool breakLoop = false;
        for (int i = 0;i<map->rows;i++){
            for (int j = 0;j<map->cols;j++){
                if (map->grid[i][j].map_tp){
                    SDL_Rect* pos = map->grid[i][j].objects->object->pos;
                    if (player.pos->x == pos->x && player.pos->y == pos->y && openDoor(player.inventory,i,j,map->level)){
                        dimMap(map->grid[i][j].map_tp,&rows,&cols);
                        pos_x = MAX(0,SIZE_WALL_W*(NB_WALL_W-cols)/2);
                        pos_y = MAX(0,SIZE_WALL_H*(NB_WALL_H-rows)/2);
                        player.pos->x = SIZE_WALL_W+pos_x;
                        player.pos->y = SIZE_WALL_H+pos_y; 
                        char* tmp = map->grid[i][j].map_tp;
                        int numLevel = numLevelFromChar(tmp);
                        if (!maps[numLevel-1]) maps[numLevel-1] = FileToMap(tmp,pos_x,pos_y,textures);
                        map = maps[numLevel-1];
                        breakLoop = true;
                    }
                }
            }
            if (breakLoop) break;
        }
        //////
        if ( SDL_PollEvent(&event) ) { 
            switch (event.type) {
                case SDL_QUIT: 
                    running=0;
                    break;
                case SDL_KEYUP:
                    offset = 0;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym){
                        case SDLK_ESCAPE:
                            running = 0;
                            break;
                        case SDLK_a:
                            dark = !dark;
                            break;

                        case SDLK_e://Prend la clé dans l'inventaire
                            interact(map,&player);
                            break;

                        case SDLK_UP:
                            // Vérifier la collision avec le mur avant de mettre à jour la position du player
                            if (!collisions(&(SDL_Rect){player.pos->x, player.pos->y - VITESSE, player.pos->w, player.pos->h},map)) {
                                if ( pos_y || wall0.y == 0 || (wallf.y == (NB_WALL_H-1)*SIZE_WALL_H && player.pos->y > SIZE_WALL_H*(NB_WALL_H-1)/2 ) ){
                                    player.pos->y -= VITESSE;
                                }
                                else {
                                    decalageMap(map,DOWN);
                                    decalageMur(&wall0,DOWN);
                                    decalageMur(&wallf,DOWN);
                                }
                            }
                            direction = 1;offset++;offset %= 4;
                            break;
                        case SDLK_DOWN:
                            // Vérifier la collision avec le mur avant de mettre à jour la position du player
                            if (!collisions(&(SDL_Rect){player.pos->x, player.pos->y + VITESSE, player.pos->w, player.pos->h},map)) {
                                if ( pos_y || wallf.y == (NB_WALL_H-1)*SIZE_WALL_H || (wall0.y == 0 && player.pos->y < SIZE_WALL_H*(NB_WALL_H-1)/2) ){
                                    player.pos->y += VITESSE;
                                }
                                else {
                                    decalageMap(map,UP);
                                    decalageMur(&wall0,UP);
                                    decalageMur(&wallf,UP);
                                }
                            }
                            direction = 0;offset++;offset %= 4;
                            break;
                        case SDLK_LEFT:
                            // Vérifier la collision avec le mur avant de mettre à jour la position du player
                            if (!collisions(&(SDL_Rect){player.pos->x - VITESSE, player.pos->y, player.pos->w, player.pos->h},map)) {
                                if ( pos_x || wall0.x == 0 || (wallf.x == (NB_WALL_W-1)*SIZE_WALL_W && player.pos->x > SIZE_WALL_W*(NB_WALL_W-1)/2) ){
                                    player.pos->x -= VITESSE;
                                }
                                else {
                                    decalageMap(map,RIGHT);
                                    decalageMur(&wall0,RIGHT);
                                    decalageMur(&wallf,RIGHT);
                                }
                            }
                            direction = 2;offset++;offset %= 4;
                            break;
                        case SDLK_RIGHT:
                            // Vérifier la collision avec le mur avant de mettre à jour la position du player
                            if (!collisions(&(SDL_Rect){player.pos->x + VITESSE, player.pos->y, player.pos->w, player.pos->h},map)) {
                                if ( pos_x || wallf.x == (NB_WALL_W-1)*SIZE_WALL_W || (wall0.x == 0 && player.pos->x < SIZE_WALL_W*(NB_WALL_W-1)/2) ){
                                    player.pos->x += VITESSE;
                                }
                                else {
                                    decalageMap(map,LEFT);
                                    decalageMur(&wall0,LEFT);
                                    decalageMur(&wallf,LEFT);
                                }
                                
                            }
                            direction = 3;offset++;offset %= 4;
                            break;
                    }
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawMap(renderer,map);
        SDL_Rect spriteRect = {.x = offset*spriteWidth ,.y = direction*spriteHeight, .w = spriteWidth, .h = spriteHeight};
        drawPlayer(renderer,&player,&spriteRect);

        if (dark) drawTransparency(renderer,player);
        
        SDL_RenderPresent(renderer);
    }

    freeSprites(textures);
    freeMaps(maps);
    
    freeListObj(player.inventory);
    SDL_DestroyTexture(spriteTexture);

    quitSDL(window, renderer);
    return 0;
}

