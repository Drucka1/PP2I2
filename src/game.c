#include "../include/struct.h"
#include "../include/aux.h"
#include "../include/init.h"

void drawWall(SDL_Renderer* renderer,SDL_Rect* mur,int nb_mur){ 
    SDL_SetRenderDrawColor(renderer, 0,0,0, 255);
    SDL_RenderFillRects(renderer, mur,nb_mur);
}

SDL_bool collisions(SDL_Rect* player, SDL_Rect* murs, int nb_murs){
    for (int i = 0;i<nb_murs;i++){
        if (SDL_HasIntersection(player,&murs[i])){
            return SDL_TRUE;
        }
    }
    return SDL_FALSE;
}

void decalage_mur(SDL_Rect* murs,int nb_murs,int decalage){
    for (int i = 0;i<nb_murs;i++){
        if (decalage == 0){
            murs[i].y += VITESSE;
        } else if (decalage == 1){
            murs[i].y -= VITESSE;
        } else if (decalage == 2){
            murs[i].x += VITESSE;
        } else {
            murs[i].x -= VITESSE;
        }
    }
}

int main(int argc, char* argv[]) {

    if(SDL_Init(SDL_INIT_EVERYTHING)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in init: %s", SDL_GetError());
    }
    atexit(SDL_Quit);

    //Window
    SDL_Window* window = SDL_CreateWindow("Essai",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WINDOW_WIDTH,WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //Terrain
    int cols,rows;
    int** terrain = FileToMap(argv[1],&rows,&cols);
    int pos_x = MAX(0,SIZE_WALL_W*(NB_WALL_W-cols)/2);
    int pos_y = MAX(0,SIZE_WALL_H*(NB_WALL_H-rows)/2);
    SDL_Rect* murs = init_terrain(terrain,rows,cols,pos_x,pos_y);
    int nb_murs = sommeMatrice(terrain,rows,cols);

    //Player
    mob player = {SIZE_WALL_W+pos_x,SIZE_WALL_H+pos_y,SIZE_WALL_W,SIZE_WALL_H};
    SDL_Surface* spriteSurface = SDL_LoadBMP("./assets/player.bmp");
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

    int spriteFullWIdth,spriteFullHeight;
    if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWIdth, &spriteFullHeight)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in query texture: %s", SDL_GetError());
        exit(-1);
    }

    int spriteWidth = 141;
    int spriteHeight = 221;
    SDL_Event event;
    int running=1;int offset = 0;int direction = 0;

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
                        case SDLK_UP:
                        // Vérifier la collision avec le mur avant de mettre à jour la position du player
                        if (!collisions(&(SDL_Rect){player.x, player.y - VITESSE, player.width, player.height},murs,nb_murs)) {
                            if ( pos_y || murs[0].y == 0 || (murs[nb_murs-1].y == (NB_WALL_H-1)*SIZE_WALL_H && player.y > SIZE_WALL_H*(NB_WALL_H-1)/2 ) ){
                                player.y -= VITESSE;
                            }
                            else {
                                decalage_mur(murs,nb_murs,0);
                            }
                        }
                        direction = 1;offset++;offset %= 4;
                        break;
                    case SDLK_DOWN:
                        // Vérifier la collision avec le mur avant de mettre à jour la position du player
                        if (!collisions(&(SDL_Rect){player.x, player.y + VITESSE, player.width, player.height},murs,nb_murs)) {
                            if ( pos_y || murs[nb_murs-1].y == (NB_WALL_H-1)*SIZE_WALL_H || (murs[0].y == 0 && player.y < SIZE_WALL_H*(NB_WALL_H-1)/2) ){
                                player.y += VITESSE;
                            }
                            else {
                                decalage_mur(murs,nb_murs,1);
                            }
                        }
                        direction = 0;offset++;offset %= 4;
                        break;
                    case SDLK_LEFT:
                        // Vérifier la collision avec le mur avant de mettre à jour la position du player
                        if (!collisions(&(SDL_Rect){player.x - VITESSE, player.y, player.width, player.height},murs,nb_murs)) {
                            if ( pos_x || murs[0].x == 0 || (murs[nb_murs-1].x == (NB_WALL_W-1)*SIZE_WALL_W && player.x > SIZE_WALL_W*(NB_WALL_W-1)/2) ){
                                player.x -= VITESSE;
                            }
                            else {
                                decalage_mur(murs,nb_murs,2);
                            }
                        }
                        direction = 2;offset++;offset %= 4;
                        break;
                    case SDLK_RIGHT:
                        // Vérifier la collision avec le mur avant de mettre à jour la position du player
                        if (!collisions(&(SDL_Rect){player.x + VITESSE, player.y, player.width, player.height},murs,nb_murs)) {
                            if ( pos_x || murs[nb_murs-1].x == (NB_WALL_W-1)*SIZE_WALL_W || (murs[0].x == 0 && player.x < SIZE_WALL_W*(NB_WALL_W-1)/2) ){
                                player.x += VITESSE;
                            }
                            else {
                                decalage_mur(murs,nb_murs,3);
                            }
                            
                        }
                        direction = 3;offset++;offset %= 4;
                        break;
                    }
            }
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        drawWall(renderer, murs,nb_murs);

        SDL_Rect spriteRect = {.x = offset*spriteWidth ,.y = direction*spriteHeight, .w = spriteWidth, .h = spriteHeight};
        SDL_Rect destRect = {player.x, player.y, SIZE_WALL_W, SIZE_WALL_H};
        if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
        }
        
        SDL_RenderPresent(renderer);
    }

    free(murs);
    free_terrain(terrain, rows);
    SDL_DestroyTexture(spriteTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
