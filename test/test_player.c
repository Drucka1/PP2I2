#include "../Unity/unity.h"
#include "../src/game/player.h"
#include "../src/game/game.h"

// This function is run before each test
void setUp_player(void) {
   
}

// This function is run after each test
void tearDown_player(void) {
}

// Example test case
void test_init() {
    SDL_Renderer *renderer = NULL;
    SDL_Window *window = NULL;
    TTF_Font * font = NULL;
    initSDL(&window, &renderer,&font);
    SDL_Texture **textures = loadTextures(renderer);
    Map **rooms = loadRooms(textures);
    Map *map = rooms[0];
    Entity *player = loadPlayer(map->spawnIndex, renderer);

    TEST_ASSERT_NOT_NULL(player);
    TEST_ASSERT_EQUAL(player->index.i, map->spawnIndex.i);
    TEST_ASSERT_EQUAL(player->index.j, map->spawnIndex.j);
    
    freeGame(player, textures, rooms);
    quitSDL(window, renderer,font);
}
void test_movePlayer(){
    SDL_Renderer *renderer = NULL;
    SDL_Window *window = NULL;
    TTF_Font * font = NULL;
    initSDL(&window, &renderer,&font);
    SDL_Texture **textures = loadTextures(renderer);
    Map **rooms = loadRooms(textures);
    Map *map = rooms[0];
    Entity *player = loadPlayer(map->spawnIndex, renderer);


    Index dest = {player->index.i + 1, player->index.j};
    movePlayer(player, map, dest);
    TEST_ASSERT_EQUAL(player->index.i, dest.i);
    TEST_ASSERT_EQUAL(player->index.j, dest.j);



    freeGame(player, textures, rooms);
    quitSDL(window, renderer,font);
}
void test_teleport() {
    SDL_Renderer *renderer = NULL;
    SDL_Window *window = NULL;
    TTF_Font * font = NULL;
    initSDL(&window, &renderer,&font);
    SDL_Texture **textures = loadTextures(renderer);
    Map **rooms = loadRooms(textures);
    Map *map = rooms[0];
    Entity *player = loadPlayer(map->spawnIndex, renderer);



    int room = 1;
    Index spawnIndex = rooms[room]->spawnIndex;
    teleport(room, spawnIndex, player, map, rooms);
    TEST_ASSERT_EQUAL(player->index.i, spawnIndex.i);
    TEST_ASSERT_EQUAL(player->index.j, spawnIndex.j);


    freeGame(player, textures, rooms);
    quitSDL(window, renderer,font);
}

void test_freePlayer(Entity *player) {
    SDL_Renderer *renderer = NULL;
    SDL_Window *window = NULL;
    TTF_Font * font = NULL;
    initSDL(&window, &renderer,&font);
    SDL_Texture **textures = loadTextures(renderer);
    Map **rooms = loadRooms(textures);
    Map *map = rooms[0];
    Entity *player = loadPlayer(map->spawnIndex, renderer);
    freePlayer(player);
    TEST_ASSERT_NULL(player);

     freeGame(player, textures, rooms);
    quitSDL(window, renderer,font);
}

int main_player(void) {
    UNITY_BEGIN();
    RUN_TEST(test_init);
    RUN_TEST(test_movePlayer);
    RUN_TEST(test_teleport);
    RUN_TEST(test_freePlayer);
    return UNITY_END();
}
