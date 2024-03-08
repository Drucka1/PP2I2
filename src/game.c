#include "../include/struct.h"
#include "../include/aux.h"
#include "../include/init.h"

void drawSquare(SDL_Renderer* renderer,SDL_Rect* mur,int nb_mur){ 
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

int main() {
    int cols,rows;
    int** terrain = FileToMap("./assets/terrain.txt",&rows,&cols);
    SDL_Rect* murs = init_terrain(terrain,rows,cols);
    int nb_murs = sommeMatrice(terrain,rows,cols);
    mob player = {5*SIZE_WALL_W,5*SIZE_WALL_H,SIZE_WALL_W,SIZE_WALL_H};

    if(SDL_Init(SDL_INIT_EVERYTHING)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in init: %s", SDL_GetError());
    }
    atexit(SDL_Quit);
    
    SDL_Event event;
    int running=1;

    if (SDL_VideoInit(NULL) < 0) { exit(1); }// SDL_VideoInit renvoie 0 en cas de succes
    
    SDL_Window* window = SDL_CreateWindow("Essai",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WINDOW_WIDTH,WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    
    // Créer un renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    while (running ) { //boucle principale 
        if ( SDL_PollEvent(&event) ) { // scrute sans cesse les évènements et renvoie 1
            switch (event.type) {
                case SDL_QUIT: //évènement fermeture de la fenêtre
                    running=0;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym){
                        case SDLK_UP:
                        // Vérifier la collision avec le mur avant de mettre à jour la position du player
                        if (!collisions(&(SDL_Rect){player.x, player.y - VITESSE, player.width, player.height},murs,nb_murs)) {
                            if (murs[rows-1].y == 0 || (murs[nb_murs-1].y == (NB_WALL_H-1)*SIZE_WALL_H && player.y > SIZE_WALL_H*(NB_WALL_H-1)/2 )){
                                player.y -= VITESSE;
                            }
                            else {
                                decalage_mur(murs,nb_murs,0);
                            }
                        }
                        break;
                    case SDLK_DOWN:
                        // Vérifier la collision avec le mur avant de mettre à jour la position du player
                        if (!collisions(&(SDL_Rect){player.x, player.y + VITESSE, player.width, player.height},murs,nb_murs)) {
                            if ( murs[nb_murs-1].y == (NB_WALL_H-1)*SIZE_WALL_H || (murs[rows-1].y == 0 && player.y < SIZE_WALL_H*(NB_WALL_H-1)/2)){
                                player.y += VITESSE;
                            }
                            else {
                                decalage_mur(murs,nb_murs,1);
                            }
                        }
                        break;
                    case SDLK_LEFT:
                        // Vérifier la collision avec le mur avant de mettre à jour la position du player
                        if (!collisions(&(SDL_Rect){player.x - VITESSE, player.y, player.width, player.height},murs,nb_murs)) {
                            if (murs[0].x == 0 || (murs[rows-1].x == (NB_WALL_W-1)*SIZE_WALL_W && player.x > SIZE_WALL_W*(NB_WALL_W-1)/2)){
                                player.x -= VITESSE;
                            }
                            else {
                                decalage_mur(murs,nb_murs,2);
                            }
                        }
                        break;
                    case SDLK_RIGHT:
                        // Vérifier la collision avec le mur avant de mettre à jour la position du player
                        if (!collisions(&(SDL_Rect){player.x + VITESSE, player.y, player.width, player.height},murs,nb_murs)) {
                            if (murs[rows-1].x == (NB_WALL_W-1)*SIZE_WALL_W || (murs[0].x == 0 && player.x < SIZE_WALL_W*(NB_WALL_W-1)/2)){
                                player.x += VITESSE;
                            }
                            else {
                                decalage_mur(murs,nb_murs,3);
                            }
                            
                        }
                        break;
                    }
            }
            printf("%lf %lf \n",player.x, player.y);
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Dessiner le carré aux coordonnées (100, 100) de taille (50, 50)
        drawSquare(renderer, murs,nb_murs);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect character = {player.x, player.y, player.width, player.height};
        SDL_RenderFillRect(renderer, &character);

        // Mettre à jour le rendu
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(0);
}
