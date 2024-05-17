#include "../include/game.h"

int main(){

    SDL_Window *window;
    SDL_Renderer *renderer;
    initSDL(&window, &renderer);

    //Initialisation du player
    Entity* player;
    char filename[100] = "assets/level/save/playerStatus.txt";
    bool load_save = false;
    int current_level = 1;
    
    if(access(filename, F_OK) != 0){
        player = malloc(sizeof(Entity));
        player->inventory = NULL;
    }
    else {
        load_save = true;
        player = getPlayerStatus(&current_level);
    }
    player->texture = load_sprite(renderer, "./assets/sprite/player.png");

    int spriteFullWIdth,spriteFullHeight;
    if (SDL_QueryTexture(player->texture, NULL, NULL, &spriteFullWIdth, &spriteFullHeight)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in query texture: %s", SDL_GetError());
        exit(-1);
    }
    //

    //Determine le chemin du level a afficher
    char path_level[80] = "assets/level/save/level";
    char lvl[10];
    sprintf(lvl,"%d",current_level);
    strcat(path_level,lvl);
    strcat(path_level,".txt");
    if(access(path_level, F_OK) != 0) {
        strcpy(path_level, "assets/level/default/level");
        strcat(path_level,lvl);
        strcat(path_level,".txt");
    }
    //

    //Initialisation de la map
    int cols,rows;
    dimMap(path_level,&rows,&cols);
    int pos_x = MAX(0,SIZE_WALL_W*(NB_WALL_W-cols)/2);
    int pos_y = MAX(0,SIZE_WALL_H*(NB_WALL_H-rows)/2);
    
    SDL_Texture** textures = malloc(sizeof(SDL_Texture*)*NB_SPRITES);
    textures[WALL] = load_sprite(renderer,"assets/sprite/wall.png");
    textures[GROUND] = load_sprite(renderer,"assets/sprite/ground.png");
    textures[DOOR] = load_sprite(renderer,"assets/sprite/door2.png");
    textures[KEY] = load_sprite(renderer,"assets/sprite/key.png");
    textures[LEVER] = load_sprite(renderer,"assets/sprite/lever.png");
    textures[ICE] = load_sprite(renderer,"assets/sprite/ice.png");
    textures[PUSH]=load_sprite(renderer,"assets/push.png");

    Map* map = FileToMap(path_level,pos_x,pos_y,textures);
    Tuple walli = {0,0};
    Tuple wallf = {(cols-1)*SIZE_WALL_W,(rows-1)*SIZE_WALL_H};
    //

    if (!load_save) {
        player->pos = malloc(sizeof(SDL_Rect));
        player->pos->x = SIZE_WALL_W+pos_x;
        player->pos->y = SIZE_WALL_H+pos_y;
        player->pos->h = SIZE_WALL_W;
        player->pos->w = SIZE_WALL_W;
    }


    int spriteWidth = spriteFullWIdth/4;
    int spriteHeight = spriteFullHeight/4;
    SDL_Event event;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    int running=1;int offset = 0;int direction = 0;
    int dark = !true;
    int isIcy =false;
    int directionInitiale = 0;
    int tempcont= 0; //variable qui sert à pousser l'objet jusqu'au bout
    ///////ZONE 51 ///////



    /////////ZONE 51 ///////
    while (running) { 
       
        //Verfie si le joueur peut changer de niveau/se tp
        bool breakLoop = false;
        for (int i = 0;i<map->rows;i++){
            for (int j = 0;j<map->cols;j++){
                if (map->grid[i][j].map_tp){
                    SDL_Rect* pos = map->grid[i][j].objects->object->pos;
                    if (player->pos->x == pos->x && player->pos->y == pos->y && openDoor(player->inventory,i,j,map->level)){
                        MapToFile(map);

                        char* lvl = map->grid[i][j].map_tp;
                        char path_level[100] = "assets/level/save/";
                        strcat(path_level,lvl);
                        if(access(path_level, F_OK) != 0) {
                            strcpy(path_level, "assets/level/default/");
                            strcat(path_level,lvl);
                        }

                        dimMap(path_level,&rows,&cols);
                        pos_x = MAX(0,SIZE_WALL_W*(NB_WALL_W-cols)/2);
                        pos_y = MAX(0,SIZE_WALL_H*(NB_WALL_H-rows)/2);
                        player->pos->x = SIZE_WALL_W+pos_x;
                        player->pos->y = SIZE_WALL_H+pos_y; 
                        freeMap(map);
                        map = FileToMap(path_level,pos_x,pos_y,textures);
                        breakLoop = true;
                    }
                }
            }
            if (breakLoop) break;
        }
        //

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
                            savePlayerStatus(player,map->level);
                            break;
                        case SDLK_a:
                            dark = !dark;
                            break;
                        case SDLK_i:
                            isIcy = !isIcy;
                            break;
                        
                        
                        case SDLK_e://Prend la clé dans l'inventaire
                            interact(map,player);
                            break;

                        case SDLK_UP:
                            if(isIcy==1){ //Si le joueur est sur de la glace on ne peut pas changer de direction
                                break;
                            }
                            //Verification si le joueur va vers un objet à pousser(istype dans game.c)
                            if(istype(&(SDL_Rect){player.pos->x, player.pos->y - VITESSE, player.pos->w, player.pos->h},map,PUSH)){
                                //On verifie si l'objet est poussable dans la direction du joueur(ici UP,ispushable dans game.c)
                                if (ispushable(&(SDL_Rect){player.pos->x, player.pos->y, player.pos->w, player.pos->h}, map, UP)) {
                                    //On va faire un do-while pour pousser l'objet puis verifier si il est sur de la glace pour le pousser jusqu'aux bout 
                                    do {
                                        //On appelle la fonction Push qui va pousser l'objet(voir struct.c je sais pas si la fonction doit etre la mais bon)
                                        Push(map, &(SDL_Rect){player.pos->x, player.pos->y - VITESSE*tempcont, player.pos->w, player.pos->h}, UP, pos_x, pos_y);
                                        tempcont++;
                                    } while (ispushable(&(SDL_Rect){player.pos->x, player.pos->y - VITESSE*tempcont, player.pos->w, player.pos->h}, map, UP) && istype(&(SDL_Rect){player.pos->x, player.pos->y - VITESSE*(tempcont+1), player.pos->w, player.pos->h}, map, ICE));
                                    tempcont = 0;
                                }
                                else{
                                    direction = 1;offset++;offset %= 4;
                                break;
                                }
                            }
                            // Vérifier si le joueur se deplace vers de la glace
                            if(istype(&(SDL_Rect){player.pos->x, player.pos->y - VITESSE, player.pos->w, player.pos->h},map,ICE)){
                                if(isIcy==0){ //Si le joueur ne glissait pas avant on le fait glisser
                                    isIcy=!isIcy;
                                    directionInitiale= UP; //directionInitiale sert a savoir dans quelle direction le joueur glisse
                                }
                            }
                            // Vérifier la collision avec le mur avant de mettre à jour la position du player
                            if (!collisions(&(SDL_Rect){player->pos->x, player->pos->y - VITESSE, player->pos->w, player->pos->h},map)) {
                                if ( pos_y || walli.y == 0 || (wallf.y == (NB_WALL_H-1)*SIZE_WALL_H && player->pos->y > SIZE_WALL_H*(NB_WALL_H-1)/2 ) ){
                                    player->pos->y -= VITESSE;
                                }
                                else {
                                    decalageMap(map,DOWN);
                                    decalageMur(&walli,DOWN);
                                    decalageMur(&wallf,DOWN);
                                }
                            }
                            direction = 1;offset++;offset %= 4;
                            break;
                        case SDLK_DOWN:
                            if(isIcy==1){
                                break;
                            }
                            if(istype(&(SDL_Rect){player.pos->x, player.pos->y + VITESSE, player.pos->w, player.pos->h},map,PUSH)){
                                if(ispushable(&(SDL_Rect){player.pos->x, player.pos->y, player.pos->w, player.pos->h},map,DOWN)){
                                    do {
                                        Push(map, &(SDL_Rect){player.pos->x, player.pos->y +VITESSE*tempcont, player.pos->w, player.pos->h}, DOWN, pos_x, pos_y);
                                        tempcont++;
                                    } while (ispushable(&(SDL_Rect){player.pos->x, player.pos->y + VITESSE*tempcont, player.pos->w, player.pos->h}, map, DOWN) && istype(&(SDL_Rect){player.pos->x, player.pos->y + VITESSE*(tempcont+1), player.pos->w, player.pos->h}, map, ICE));
                                    tempcont = 0;
                                }
                                else{
                                    direction = 0;offset++;offset %= 4;
                                break;
                                }
                            }
                            if(istype(&(SDL_Rect){player.pos->x, player.pos->y + VITESSE, player.pos->w, player.pos->h},map,ICE)){
                                if(isIcy==0){
                                    isIcy=!isIcy;
                                    directionInitiale = DOWN;
                                }
                            }
                            
                            // Vérifier la collision avec le mur avant de mettre à jour la position du player
                            if (!collisions(&(SDL_Rect){player->pos->x, player->pos->y + VITESSE, player->pos->w, player->pos->h},map)) {
                                if ( pos_y || wallf.y == (NB_WALL_H-1)*SIZE_WALL_H || (walli.y == 0 && player->pos->y < SIZE_WALL_H*(NB_WALL_H-1)/2) ){
                                    player->pos->y += VITESSE;
                                }
                                else {
                                    decalageMap(map,UP);
                                    decalageMur(&walli,UP);
                                    decalageMur(&wallf,UP);
                                }
                            }
                            direction = 0;offset++;offset %= 4;
                            break;
                        case SDLK_LEFT:
                            if(isIcy==1){
                                break;
                            }
                            if(istype(&(SDL_Rect){player.pos->x - VITESSE, player.pos->y, player.pos->w, player.pos->h},map,PUSH)){
                                if(ispushable(&(SDL_Rect){player.pos->x, player.pos->y, player.pos->w, player.pos->h},map,LEFT)){
                                    do {
                                        Push(map, &(SDL_Rect){player.pos->x - VITESSE*tempcont, player.pos->y, player.pos->w, player.pos->h}, LEFT, pos_x, pos_y);
                                        tempcont++;
                                    } while (ispushable(&(SDL_Rect){player.pos->x - VITESSE*tempcont, player.pos->y, player.pos->w, player.pos->h}, map, LEFT) && istype(&(SDL_Rect){player.pos->x - VITESSE*(tempcont+1), player.pos->y, player.pos->w, player.pos->h}, map, ICE));
                                    tempcont = 0;
                                }
                                else{
                                    direction = 2;offset++;offset %= 4;
                                break;
                                }
                            }



                            if (istype(&(SDL_Rect){player.pos->x - VITESSE, player.pos->y, player.pos->w, player.pos->h},map,PUSH)|| istype(&(SDL_Rect){player.pos->x - VITESSE, player.pos->y, player.pos->w, player.pos->h},map,KEY)){
                                dark=!dark;
                            }
                            if(istype(&(SDL_Rect){player.pos->x - VITESSE, player.pos->y, player.pos->w, player.pos->h},map,ICE)){
                                if(isIcy==0){
                                    isIcy=!isIcy;
                                    directionInitiale = LEFT;
                                }
                            }
                            
                            // Vérifier la collision avec le mur avant de mettre à jour la position du player
                            if (!collisions(&(SDL_Rect){player->pos->x - VITESSE, player->pos->y, player->pos->w, player->pos->h},map)) {
                                if ( pos_x || walli.x == 0 || (wallf.x == (NB_WALL_W-1)*SIZE_WALL_W && player->pos->x > SIZE_WALL_W*(NB_WALL_W-1)/2) ){
                                    player->pos->x -= VITESSE;
                                }
                                else {
                                    decalageMap(map,RIGHT);
                                    decalageMur(&walli,RIGHT);
                                    decalageMur(&wallf,RIGHT);
                                }
                            }
                            direction = 2;offset++;offset %= 4;
                            break;
                        case SDLK_RIGHT:
                            if(isIcy==1){
                                break;
                            }
                            if(istype(&(SDL_Rect){player.pos->x + VITESSE, player.pos->y, player.pos->w, player.pos->h},map,PUSH)){
                                if(ispushable(&(SDL_Rect){player.pos->x, player.pos->y, player.pos->w, player.pos->h},map,RIGHT)){
                                    do {
                                        Push(map, &(SDL_Rect){player.pos->x + VITESSE*tempcont, player.pos->y, player.pos->w, player.pos->h}, RIGHT, pos_x, pos_y);
                                        tempcont++;
                                    } while (ispushable(&(SDL_Rect){player.pos->x + VITESSE*tempcont, player.pos->y, player.pos->w, player.pos->h}, map, RIGHT) && istype(&(SDL_Rect){player.pos->x + VITESSE*(tempcont+1), player.pos->y, player.pos->w, player.pos->h}, map, ICE));
                                    tempcont = 0;
                                }
                                else{
                                    direction = 3;offset++;offset %= 4;
                                break;
                                }
                            }

                            if(istype(&(SDL_Rect){player.pos->x + VITESSE, player.pos->y, player.pos->w, player.pos->h},map,ICE)){
                                if(isIcy==0){
                                    isIcy=!isIcy;
                                    directionInitiale = RIGHT;
                                }
                            }
                            
                            // Vérifier la collision avec le mur avant de mettre à jour la position du player
                            if (!collisions(&(SDL_Rect){player->pos->x + VITESSE, player->pos->y, player->pos->w, player->pos->h},map)) {
                                if ( pos_x || wallf.x == (NB_WALL_W-1)*SIZE_WALL_W || (walli.x == 0 && player->pos->x < SIZE_WALL_W*(NB_WALL_W-1)/2) ){
                                    player->pos->x += VITESSE;
                                }
                                else {
                                    decalageMap(map,LEFT);
                                    decalageMur(&walli,LEFT);
                                    decalageMur(&wallf,LEFT);
                                }
                                
                            }
                            direction = 3;offset++;offset %= 4;
                            break;
                    }
            }
        }
        if(directionInitiale!=0){ //Si le joueur est sur de la glace (mis ici car je suis sur )
            switch(directionInitiale){
                case UP: // je commente que ce cas la car les autres sont les mêmes
                    if (!collisions(&(SDL_Rect){player.pos->x, player.pos->y - VITESSE, player.pos->w, player.pos->h},map)&&!istype(&(SDL_Rect){player.pos->x, player.pos->y - VITESSE, player.pos->w, player.pos->h},map,PUSH)) { //check de colisions avec les murs comme dans les deplacements classiques
                        if ( pos_y || wall0.y == 0 || (wallf.y == (NB_WALL_H-1)*SIZE_WALL_H && player.pos->y > SIZE_WALL_H*(NB_WALL_H-1)/2 ) ){
                            player.pos->y -= VITESSE;
                        }
                        else {
                            decalageMap(map,DOWN);
                            decalageMur(&walli,DOWN);
                            decalageMur(&wallf,DOWN);
                        }
                    }
                    else{ //si il y a collision avec un mur on arrete de glisser et directionInitiale=0 pour ne pas refaire le test
                        isIcy=!isIcy;
                        directionInitiale=0;
                    }
                    if(!istype(&(SDL_Rect){player.pos->x, player.pos->y, player.pos->w, player.pos->h},map,ICE)){ //si le joueur n'est plus sur de la glace on arrete de glisser + directionInitiale=0
                        isIcy=!isIcy;
                        directionInitiale=0;
                    }
                    break;
                case DOWN:
                    if (!collisions(&(SDL_Rect){player.pos->x, player.pos->y + VITESSE, player.pos->w, player.pos->h},map)&&!istype(&(SDL_Rect){player.pos->x, player.pos->y + VITESSE, player.pos->w, player.pos->h},map,PUSH)){
                        if ( pos_y || wallf.y == (NB_WALL_H-1)*SIZE_WALL_H || (wall0.y == 0 && player.pos->y < SIZE_WALL_H*(NB_WALL_H-1)/2) ){
                            player.pos->y += VITESSE;
                        }
                        else {
                            decalageMap(map,UP);
                            decalageMur(&walli,UP);
                            decalageMur(&wallf,UP);
                        }
                    }
                    else{
                        isIcy=!isIcy;
                        directionInitiale=0;
                    }
                    if(!istype(&(SDL_Rect){player.pos->x, player.pos->y, player.pos->w, player.pos->h},map,ICE)){
                        isIcy=!isIcy;
                        directionInitiale=0;
                    }
                    break;
                case LEFT:
                    if (!collisions(&(SDL_Rect){player.pos->x-VITESSE , player.pos->y, player.pos->w, player.pos->h},map)&&!istype(&(SDL_Rect){player.pos->x-VITESSE , player.pos->y, player.pos->w, player.pos->h},map,PUSH)){
                        if ( pos_x || wall0.x == 0 || (wallf.x == (NB_WALL_W-1)*SIZE_WALL_W && player.pos->x > SIZE_WALL_W*(NB_WALL_W-1)/2) ){
                            player.pos->x -= VITESSE;
                        }
                        else {
                            decalageMap(map,RIGHT);
                            decalageMur(&walli,RIGHT);
                            decalageMur(&wallf,RIGHT);
                        }
                    }
                    else{
                        isIcy=!isIcy;
                        directionInitiale=0;
                    }
                    if(!istype(&(SDL_Rect){player.pos->x, player.pos->y, player.pos->w, player.pos->h},map,ICE)){
                        isIcy=!isIcy;
                        directionInitiale=0;
                    }
                    break;
                case RIGHT:
                    if (!collisions(&(SDL_Rect){player.pos->x + VITESSE, player.pos->y, player.pos->w, player.pos->h},map)&&!istype(&(SDL_Rect){player.pos->x + VITESSE, player.pos->y, player.pos->w, player.pos->h},map,PUSH)){
                        if ( pos_x || wallf.x == (NB_WALL_W-1)*SIZE_WALL_W || (wall0.x == 0 && player.pos->x < SIZE_WALL_W*(NB_WALL_W-1)/2) ){
                            player.pos->x += VITESSE;
                        }
                        else {
                            decalageMap(map,LEFT);
                            decalageMur(&walli,LEFT);
                            decalageMur(&wallf,LEFT);
                        }
                    }
                    else{
                        isIcy=!isIcy;
                        directionInitiale=0;
                    }
                    if(!istype(&(SDL_Rect){player.pos->x, player.pos->y, player.pos->w, player.pos->h},map,ICE)){
                        isIcy=!isIcy;
                        directionInitiale=0;
                    }
                    break;
                // sleep(1);
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawMap(renderer,map);
        SDL_Rect spriteRect = {.x = offset*spriteWidth ,.y = direction*spriteHeight, .w = spriteWidth, .h = spriteHeight};
        drawPlayer(renderer,player,&spriteRect);

        if (dark) drawTransparency(renderer,player);
        
        SDL_RenderPresent(renderer);
    }

    freeSprites(textures);
    freeMap(map);
    freePlayer(player);

    quitSDL(window, renderer);
    return 0;
}