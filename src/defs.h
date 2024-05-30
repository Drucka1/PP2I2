#ifndef __DEFS_H__
#define __DEFS_H__

#define TILE_SIZE 80

#define MAP_MAX_HEIGHT 36 // 36
#define MAP_MAX_WIDTH 64  // 64

#define GRID_MAX_ROW 9
#define GRID_MAX_COLUMN 15

#define WINDOW_HEIGHT TILE_SIZE *GRID_MAX_ROW
#define WINDOW_WIDTH TILE_SIZE *GRID_MAX_COLUMN

// Fonctions min et max
#define MIN(i, j) ((i) < (j) ? (i) : (j))
#define MAX(i, j) ((i) > (j) ? (i) : (j))

#endif // DEBUG
