#include "../include/aux.h"
#include "../include/init.h"
#include "../include/game.h"
#include "../include/utils.h"

int main(int argc, char* argv[]) {

  (void) argc;
  (void) argv;
  
  SDL_Window *window;
  SDL_Renderer *renderer;
  initSDL(&window, &renderer);

  //Terrain
    int cols,rows;
    int** terrain = FileToMap(argv[1],&rows,&cols);
    int pos_x = MAX(0,SIZE_WALL_W*(NB_WALL_W-cols)/2);
    int pos_y = MAX(0,SIZE_WALL_H*(NB_WALL_H-rows)/2);
    SDL_Texture* spriteTextureWall = load_sprite(renderer, "./assets/wall.bmp");
    wall* murs = init_terrain(terrain,rows,cols,pos_x,pos_y,spriteTextureWall);
    int nb_murs = sommeMatrice(terrain,rows,cols);

    //Ground
    SDL_Texture* spriteTextureGround = load_sprite(renderer, "./assets/ground.bmp");

    //Player
    SDL_Texture* spriteTexture = load_sprite(renderer, "./assets/player.bmp");
    mob player;
    player.pos = &(SDL_Rect){SIZE_WALL_W+pos_x,SIZE_WALL_H+pos_y,SIZE_WALL_W,SIZE_WALL_H};
    player.sprite = spriteTexture;

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
                        if (!collisions(&(SDL_Rect){player.pos->x, player.pos->y - VITESSE, player.pos->w, player.pos->h},murs,nb_murs)) {
                            if ( pos_y || murs[0].pos->y == 0 || (murs[nb_murs-1].pos->y == (NB_WALL_H-1)*SIZE_WALL_H && player.pos->y > SIZE_WALL_H*(NB_WALL_H-1)/2 ) ){
                                player.pos->y -= VITESSE;
                            }
                            else {
                                decalage_mur(murs,nb_murs,0);
                            }
                        }
                        direction = 1;offset++;offset %= 4;
                        break;
                    case SDLK_DOWN:
                        // Vérifier la collision avec le mur avant de mettre à jour la position du player
                        if (!collisions(&(SDL_Rect){player.pos->x, player.pos->y + VITESSE, player.pos->w, player.pos->h},murs,nb_murs)) {
                            if ( pos_y || murs[nb_murs-1].pos->y == (NB_WALL_H-1)*SIZE_WALL_H || (murs[0].pos->y == 0 && player.pos->y < SIZE_WALL_H*(NB_WALL_H-1)/2) ){
                                player.pos->y += VITESSE;
                            }
                            else {
                                decalage_mur(murs,nb_murs,1);
                            }
                        }
                        direction = 0;offset++;offset %= 4;
                        break;
                    case SDLK_LEFT:
                        // Vérifier la collision avec le mur avant de mettre à jour la position du player
                        if (!collisions(&(SDL_Rect){player.pos->x - VITESSE, player.pos->y, player.pos->w, player.pos->h},murs,nb_murs)) {
                            if ( pos_x || murs[0].pos->x == 0 || (murs[nb_murs-1].pos->x == (NB_WALL_W-1)*SIZE_WALL_W && player.pos->x > SIZE_WALL_W*(NB_WALL_W-1)/2) ){
                                player.pos->x -= VITESSE;
                            }
                            else {
                                decalage_mur(murs,nb_murs,2);
                            }
                        }
                        direction = 2;offset++;offset %= 4;
                        break;
                    case SDLK_RIGHT:
                        // Vérifier la collision avec le mur avant de mettre à jour la position du player
                        if (!collisions(&(SDL_Rect){player.pos->x + VITESSE, player.pos->y, player.pos->w, player.pos->h},murs,nb_murs)) {
                            if ( pos_x || murs[nb_murs-1].pos->x == (NB_WALL_W-1)*SIZE_WALL_W || (murs[0].pos->x == 0 && player.pos->x < SIZE_WALL_W*(NB_WALL_W-1)/2) ){
                                player.pos->x += VITESSE;
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

        drawGround(renderer,spriteTextureGround,pos_x,pos_y,rows,cols);
        drawWall(renderer,murs,nb_murs);

        SDL_Rect spriteRect = {.x = offset*spriteWidth ,.y = direction*spriteHeight, .w = spriteWidth, .h = spriteHeight};
        SDL_Rect destRect = {player.pos->x, player.pos->y, SIZE_WALL_W, SIZE_WALL_H};
        if (SDL_RenderCopy(renderer, player.sprite, &spriteRect, &destRect)){
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
        }
        
        SDL_RenderPresent(renderer);
    }

    free_walls(murs,nb_murs);
    free_terrain(terrain, rows);
    SDL_DestroyTexture(spriteTexture);
    quitSDL(window, renderer);
  return 0;
}
