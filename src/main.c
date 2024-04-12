#include "../include/aux.h"
#include "../include/init.h"
#include "../include/game.h"

int main(int argc, char* argv[]){

    if(argc < 2){
        printf("Renseignez une map");
        exit(-1);
    }
    
    SDL_Window *window;
    SDL_Renderer *renderer;
    initSDL(&window, &renderer);

    //Terrain
    int cols,rows;
    List* **terrain = FileToMap(argv[1],&rows,&cols);
    int pos_x = MAX(0,SIZE_WALL_W*(NB_WALL_W-cols)/2);
    int pos_y = MAX(0,SIZE_WALL_H*(NB_WALL_H-rows)/2);
    
    SDL_Texture** textures = malloc(sizeof(SDL_Texture*)*4);
    textures[WALL] = load_sprite(renderer,"assets/wall.png");
    textures[GROUND] = load_sprite(renderer,"assets/ground.png");
    textures[DOOR] = load_sprite(renderer,"assets/door2.png");
    textures[KEY] = load_sprite(renderer,"assets/key.png");

    Map* map = initMap(terrain,rows,cols,pos_x,pos_y,textures);
    Tuple wall0 = {0,0};
    Tuple wallf = {(cols-1)*SIZE_WALL_W,(rows-1)*SIZE_WALL_H};
    freeTerrain(terrain,rows,cols);
    
    //Player
    SDL_Texture* spriteTexture = load_sprite(renderer, "./assets/player.png");
    Entity player;
    player.pos = &(SDL_Rect){SIZE_WALL_W+pos_x,SIZE_WALL_H+pos_y,SIZE_WALL_W,SIZE_WALL_H};
    player.texture = spriteTexture;

    int spriteFullWIdth,spriteFullHeight;
    if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWIdth, &spriteFullHeight)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in query texture: %s", SDL_GetError());
        exit(-1);
    }

    int spriteWidth = 141;
    int spriteHeight = 221;
    SDL_Event event;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    int running=1;int offset = 0;int direction = 0;
    int dark = true;

    while (running ) { //boucle principale 
        if ( SDL_PollEvent(&event) ) { // scrute sans cesse les évènements et renvoie 1
            switch (event.type) {
                case SDL_QUIT: //évènement fermeture de la fenêtre
                    running=0;
                    break;
                case SDL_KEYUP:
                    offset = 0;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym){
                        case SDLK_a:
                            dark = !dark;
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
        SDL_Rect destRect = {player.pos->x, player.pos->y, SIZE_WALL_W, SIZE_WALL_H};
        if (SDL_RenderCopy(renderer, player.texture, &spriteRect, &destRect)){
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
        }

        if (dark){
            drawTransparency(renderer,player);
        }
        
        SDL_RenderPresent(renderer);
    }

    freeSprites(textures,4);
    freeMap(map);
    SDL_DestroyTexture(spriteTexture);
    quitSDL(window, renderer);
    return 0;
}
