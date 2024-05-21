#include "../include/game.h"

int main(){

    SDL_Window *window;
    SDL_Renderer *renderer;
    initSDL(&window, &renderer);
    
    SDL_Texture** textures = malloc(sizeof(SDL_Texture*)*NB_SPRITES);
    textures[WALL] = load_sprite(renderer,"assets/sprite/wall.png");
    textures[GROUND] = load_sprite(renderer,"assets/sprite/ground.png");
    textures[DOOR] = load_sprite(renderer,"assets/sprite/door2.png");
    textures[KEY] = load_sprite(renderer,"assets/sprite/key.png");
    textures[LEVER] = load_sprite(renderer,"assets/sprite/lever.png");
    textures[ICE] = load_sprite(renderer,"assets/sprite/ice.png");
    textures[PUSH]=load_sprite(renderer,"assets/sprite/push.png");

    Entity* player;
    Map* map;
   
    if(access("assets/level/save/playerStatus.txt", F_OK) != 0){
        //Aucune sauvergade -> On commennce au niveau 1

        char* path_level = getPath(true,1);
    
        int offset_player_x,offset_player_y;
        posInitPlayerLevel(path_level,&offset_player_x,&offset_player_y);

        map = FileToMap(path_level,NULL,textures);
        player = malloc(sizeof(Entity));
        player->inventory = NULL;
        player->pos = malloc(sizeof(SDL_Rect));
        player->pos->x = map->offset_map.x+offset_player_x*SIZE_WALL_W;
        player->pos->y = map->offset_map.y+offset_player_y*SIZE_WALL_H;
        player->pos->h = SIZE_WALL_W;
        player->pos->w = SIZE_WALL_W;
        free(path_level);    
    }
    else {
        int current_level = 1;
        player = getPlayerStatus(&current_level);

        //Determine le chemin du level a afficher
        char* path_level = getPath(false,current_level);
        if(access(path_level, F_OK) != 0) {
            path_level = getPath(true,current_level);
        }
   
        map = FileToMap(path_level,player->pos,textures);
        free(path_level);
    }    

    player->texture = load_sprite(renderer, "assets/sprite/player.png");

    int spriteFullWIdth,spriteFullHeight;
    if (SDL_QueryTexture(player->texture, NULL, NULL, &spriteFullWIdth, &spriteFullHeight)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in query texture: %s", SDL_GetError());
        exit(-1);
    }

    int spriteWidth = spriteFullWIdth/4;
    int spriteHeight = spriteFullHeight/4;
    SDL_Event event;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    int running=1;int offset = 0;int direction = 0;
    bool dark = !true;
    bool isIcy =false;
    int directionInitiale = 0;
    int tempcont= 0; //variable qui sert à pousser l'objet jusqu'au bout
    ///////ZONE 51 ///////



    /////////ZONE 51 ///////
    while (running) { 

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
                            if(ENABLE_SAVE){
                                MapToFile(map,(player->pos->x - map->grid[0][0].objects->object->pos->x) / SIZE_WALL_W,(player->pos->y - map->grid[0][0].objects->object->pos->y) / SIZE_WALL_W);
                                savePlayerStatus(player,map->level);
                            }
                            break;
                        case SDLK_a:
                            dark = !dark;
                            break;
                        case SDLK_i:
                            isIcy = !isIcy;
                            break;    
                        case SDLK_r:
                            {   
                                char* path = getPath(false,map->level);
                                char commande[150] = "cp -f ";
                                char* defaultPath = getPath(true,map->level);
                                strcat(commande,defaultPath);
                                strcat(commande," ");
                                strcat(commande,path);
                                system(commande);

                                player->inventory = removeKeyInventory(player->inventory,map->level);
                                freeMap(map);
                                map = FileToMap(path,NULL,textures);

                                int offset_player_x,offset_player_y;
                                posInitPlayerLevel(path,&offset_player_x,&offset_player_y);
                                player->pos->x = map->offset_map.x+offset_player_x*SIZE_WALL_W;
                                player->pos->y = map->offset_map.y+offset_player_y*SIZE_WALL_H;
                                free(path);
                                free(defaultPath);
                                break; 
                            } 
                        case SDLK_e://Prend la clé dans l'inventaire
                            interact(&map,player,textures);
                            break;

                        case SDLK_UP:
                            if(isIcy==1){ //Si le joueur est sur de la glace on ne peut pas changer de direction
                                break;
                            }
                            //Verification si le joueur va vers un objet à pousser(istype dans game.c)
                            if(istype(&(SDL_Rect){player->pos->x, player->pos->y - VITESSE, player->pos->w, player->pos->h},map,PUSH)){
                                //On verifie si l'objet est poussable dans la direction du joueur(ici UP,ispushable dans game.c)
                                if (ispushable(&(SDL_Rect){player->pos->x, player->pos->y, player->pos->w, player->pos->h}, map, UP)) {
                                    //On va faire un do-while pour pousser l'objet puis verifier si il est sur de la glace pour le pousser jusqu'aux bout 
                                    do {
                                        //On appelle la fonction Push qui va pousser l'objet(voir struct.c je sais pas si la fonction doit etre la mais bon)
                                        Push(map, &(SDL_Rect){player->pos->x, player->pos->y - VITESSE*tempcont, player->pos->w, player->pos->h}, UP);
                                        tempcont++;
                                    } while (ispushable(&(SDL_Rect){player->pos->x, player->pos->y - VITESSE*tempcont, player->pos->w, player->pos->h}, map, UP) && istype(&(SDL_Rect){player->pos->x, player->pos->y - VITESSE*(tempcont+1), player->pos->w, player->pos->h}, map, ICE));
                                    tempcont = 0;
                                }
                                else{
                                    direction = 1;offset++;offset %= 4;
                                break;
                                }
                            }
                            // Vérifier si le joueur se deplace vers de la glace
                            if(istype(&(SDL_Rect){player->pos->x, player->pos->y - VITESSE, player->pos->w, player->pos->h},map,ICE)){
                                if(isIcy==0){ //Si le joueur ne glissait pas avant on le fait glisser
                                    isIcy=!isIcy;
                                    directionInitiale= UP; //directionInitiale sert a savoir dans quelle direction le joueur glisse
                                }
                            }
                            // Vérifier la collision avec le mur avant de mettre à jour la position du player
                            if (!collisions(&(SDL_Rect){player->pos->x, player->pos->y - VITESSE, player->pos->w, player->pos->h},map)) {
                                if ( map->offset_map.y > 0 || map->UpperLeftCorner.y == 0 || (map->BottomRightCorner.y == (NB_WALL_H-1)*SIZE_WALL_H && player->pos->y > SIZE_WALL_H*(NB_WALL_H-1)/2 ) ){
                                    player->pos->y -= VITESSE;
                                }
                                else {
                                    decalageMap(map,DOWN);
                                    decalageMur(&map->UpperLeftCorner,DOWN);
                                    decalageMur(&map->BottomRightCorner,DOWN);
                                }
                            }
                            direction = 1;offset++;offset %= 4;
                            break;
                        case SDLK_DOWN:
                            if(isIcy==1){
                                break;
                            }
                            if(istype(&(SDL_Rect){player->pos->x, player->pos->y + VITESSE, player->pos->w, player->pos->h},map,PUSH)){
                                if(ispushable(&(SDL_Rect){player->pos->x, player->pos->y, player->pos->w, player->pos->h},map,DOWN)){
                                    do {
                                        Push(map, &(SDL_Rect){player->pos->x, player->pos->y +VITESSE*tempcont, player->pos->w, player->pos->h}, DOWN);
                                        tempcont++;
                                    } while (ispushable(&(SDL_Rect){player->pos->x, player->pos->y + VITESSE*tempcont, player->pos->w, player->pos->h}, map, DOWN) && istype(&(SDL_Rect){player->pos->x, player->pos->y + VITESSE*(tempcont+1), player->pos->w, player->pos->h}, map, ICE));
                                    tempcont = 0;
                                }
                                else{
                                    direction = 0;offset++;offset %= 4;
                                break;
                                }
                            }
                            if(istype(&(SDL_Rect){player->pos->x, player->pos->y + VITESSE, player->pos->w, player->pos->h},map,ICE)){
                                if(isIcy==0){
                                    isIcy=!isIcy;
                                    directionInitiale = DOWN;
                                }
                            }
                            
                            // Vérifier la collision avec le mur avant de mettre à jour la position du player
                            if (!collisions(&(SDL_Rect){player->pos->x, player->pos->y + VITESSE, player->pos->w, player->pos->h},map)) {
                                if ( map->offset_map.y > 0 || map->BottomRightCorner.y == (NB_WALL_H-1)*SIZE_WALL_H || (map->UpperLeftCorner.y == 0 && player->pos->y < SIZE_WALL_H*(NB_WALL_H-1)/2) ){
                                    player->pos->y += VITESSE;
                                }
                                else {
                                    decalageMap(map,UP);
                                    decalageMur(&map->UpperLeftCorner,UP);
                                    decalageMur(&map->BottomRightCorner,UP);
                                }
                            }
                            direction = 0;offset++;offset %= 4;
                            break;
                        case SDLK_LEFT:
                            if(isIcy==1){
                                break;
                            }
                            if(istype(&(SDL_Rect){player->pos->x - VITESSE, player->pos->y, player->pos->w, player->pos->h},map,PUSH)){
                                if(ispushable(&(SDL_Rect){player->pos->x, player->pos->y, player->pos->w, player->pos->h},map,LEFT)){
                                    do {
                                        Push(map, &(SDL_Rect){player->pos->x - VITESSE*tempcont, player->pos->y, player->pos->w, player->pos->h}, LEFT);
                                        tempcont++;
                                    } while (ispushable(&(SDL_Rect){player->pos->x - VITESSE*tempcont, player->pos->y, player->pos->w, player->pos->h}, map, LEFT) && istype(&(SDL_Rect){player->pos->x - VITESSE*(tempcont+1), player->pos->y, player->pos->w, player->pos->h}, map, ICE));
                                    tempcont = 0;
                                }
                                else{
                                    direction = 2;offset++;offset %= 4;
                                break;
                                }
                            }

                            if(istype(&(SDL_Rect){player->pos->x - VITESSE, player->pos->y, player->pos->w, player->pos->h},map,ICE)){
                                if(isIcy==0){
                                    isIcy=!isIcy;
                                    directionInitiale = LEFT;
                                }
                            }
                            
                            // Vérifier la collision avec le mur avant de mettre à jour la position du player
                            if (!collisions(&(SDL_Rect){player->pos->x - VITESSE, player->pos->y, player->pos->w, player->pos->h},map)) {
                                if ( map->offset_map.x > 0 || map->UpperLeftCorner.x == 0 || (map->BottomRightCorner.x == (NB_WALL_W-1)*SIZE_WALL_W && player->pos->x > SIZE_WALL_W*(NB_WALL_W-1)/2) ){
                                    player->pos->x -= VITESSE;
                                }
                                else {
                                    decalageMap(map,RIGHT);
                                    decalageMur(&map->UpperLeftCorner,RIGHT);
                                    decalageMur(&map->BottomRightCorner,RIGHT);
                                }
                            }
                            direction = 2;offset++;offset %= 4;
                            break;
                        case SDLK_RIGHT:
                            if(isIcy==1){
                                break;
                            }
                            if(istype(&(SDL_Rect){player->pos->x + VITESSE, player->pos->y, player->pos->w, player->pos->h},map,PUSH)){
                                if(ispushable(&(SDL_Rect){player->pos->x, player->pos->y, player->pos->w, player->pos->h},map,RIGHT)){
                                    do {
                                        Push(map, &(SDL_Rect){player->pos->x + VITESSE*tempcont, player->pos->y, player->pos->w, player->pos->h}, RIGHT);
                                        tempcont++;
                                    } while (ispushable(&(SDL_Rect){player->pos->x + VITESSE*tempcont, player->pos->y, player->pos->w, player->pos->h}, map, RIGHT) && istype(&(SDL_Rect){player->pos->x + VITESSE*(tempcont+1), player->pos->y, player->pos->w, player->pos->h}, map, ICE));
                                    tempcont = 0;
                                }
                                else{
                                    direction = 3;offset++;offset %= 4;
                                break;
                                }
                            }

                            if(istype(&(SDL_Rect){player->pos->x + VITESSE, player->pos->y, player->pos->w, player->pos->h},map,ICE)){
                                if(isIcy==0){
                                    isIcy=!isIcy;
                                    directionInitiale = RIGHT;
                                }
                            }
                            
                            // Vérifier la collision avec le mur avant de mettre à jour la position du player
                            if (!collisions(&(SDL_Rect){player->pos->x + VITESSE, player->pos->y, player->pos->w, player->pos->h},map)) {
                                if ( map->offset_map.x > 0 || map->BottomRightCorner.x == (NB_WALL_W-1)*SIZE_WALL_W || (map->UpperLeftCorner.x == 0 && player->pos->x < SIZE_WALL_W*(NB_WALL_W-1)/2)){
                                    player->pos->x += VITESSE;
                                }
                                else {
                                    decalageMap(map,LEFT);
                                    decalageMur(&map->UpperLeftCorner,LEFT);
                                    decalageMur(&map->BottomRightCorner,LEFT);
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
                    if (!collisions(&(SDL_Rect){player->pos->x, player->pos->y - VITESSE, player->pos->w, player->pos->h},map)&&!istype(&(SDL_Rect){player->pos->x, player->pos->y - VITESSE, player->pos->w, player->pos->h},map,PUSH)) { //check de colisions avec les murs comme dans les deplacements classiques
                        if ( map->offset_map.y > 0 || map->UpperLeftCorner.y == 0 || (map->BottomRightCorner.y == (NB_WALL_H-1)*SIZE_WALL_H && player->pos->y > SIZE_WALL_H*(NB_WALL_H-1)/2 ) ){
                            player->pos->y -= VITESSE;
                        }
                        else {
                            decalageMap(map,DOWN);
                            decalageMur(&map->UpperLeftCorner,DOWN);
                            decalageMur(&map->BottomRightCorner,DOWN);
                        }
                    }
                    else{ //si il y a collision avec un mur on arrete de glisser et directionInitiale=0 pour ne pas refaire le test
                        isIcy=!isIcy;
                        directionInitiale=0;
                    }
                    if(!istype(&(SDL_Rect){player->pos->x, player->pos->y, player->pos->w, player->pos->h},map,ICE)){ //si le joueur n'est plus sur de la glace on arrete de glisser + directionInitiale=0
                        isIcy=!isIcy;
                        directionInitiale=0;
                    }
                    break;
                case DOWN:
                    if (!collisions(&(SDL_Rect){player->pos->x, player->pos->y + VITESSE, player->pos->w, player->pos->h},map)&&!istype(&(SDL_Rect){player->pos->x, player->pos->y + VITESSE, player->pos->w, player->pos->h},map,PUSH)){
                        if ( map->offset_map.y > 0 || map->BottomRightCorner.y == (NB_WALL_H-1)*SIZE_WALL_H || (map->UpperLeftCorner.y == 0 && player->pos->y < SIZE_WALL_H*(NB_WALL_H-1)/2) ){
                            player->pos->y += VITESSE;
                        }
                        else {
                            decalageMap(map,UP);
                            decalageMur(&map->UpperLeftCorner,UP);
                            decalageMur(&map->BottomRightCorner,UP);
                        }
                    }
                    else{
                        isIcy=!isIcy;
                        directionInitiale=0;
                    }
                    if(!istype(&(SDL_Rect){player->pos->x, player->pos->y, player->pos->w, player->pos->h},map,ICE)){
                        isIcy=!isIcy;
                        directionInitiale=0;
                    }
                    break;
                case LEFT:
                    if (!collisions(&(SDL_Rect){player->pos->x-VITESSE , player->pos->y, player->pos->w, player->pos->h},map)&&!istype(&(SDL_Rect){player->pos->x-VITESSE , player->pos->y, player->pos->w, player->pos->h},map,PUSH)){
                        if ( map->offset_map.x > 0 || map->UpperLeftCorner.x == 0 || (map->BottomRightCorner.x == (NB_WALL_W-1)*SIZE_WALL_W && player->pos->x > SIZE_WALL_W*(NB_WALL_W-1)/2) ){
                            player->pos->x -= VITESSE;
                        }
                        else {
                            decalageMap(map,RIGHT);
                            decalageMur(&map->UpperLeftCorner,RIGHT);
                            decalageMur(&map->BottomRightCorner,RIGHT);
                        }
                    }
                    else{
                        isIcy=!isIcy;
                        directionInitiale=0;
                    }
                    if(!istype(&(SDL_Rect){player->pos->x, player->pos->y, player->pos->w, player->pos->h},map,ICE)){
                        isIcy=!isIcy;
                        directionInitiale=0;
                    }
                    break;
                case RIGHT:
                    if (!collisions(&(SDL_Rect){player->pos->x + VITESSE, player->pos->y, player->pos->w, player->pos->h},map)&&!istype(&(SDL_Rect){player->pos->x + VITESSE, player->pos->y, player->pos->w, player->pos->h},map,PUSH)){
                        if ( map->offset_map.x > 0 || map->BottomRightCorner.x == (NB_WALL_W-1)*SIZE_WALL_W || (map->UpperLeftCorner.x == 0 && player->pos->x < SIZE_WALL_W*(NB_WALL_W-1)/2) ){
                            player->pos->x += VITESSE;
                        }
                        else {
                            decalageMap(map,LEFT);
                            decalageMur(&map->UpperLeftCorner,LEFT);
                            decalageMur(&map->BottomRightCorner,LEFT);
                        }
                    }
                    else{
                        isIcy=!isIcy;
                        directionInitiale=0;
                    }
                    if(!istype(&(SDL_Rect){player->pos->x, player->pos->y, player->pos->w, player->pos->h},map,ICE)){
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