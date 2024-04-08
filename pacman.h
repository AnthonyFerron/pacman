#ifndef PACMAN
#define PACMAN

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL.h>
#include <stdbool.h>

#define WINDOW_WIDTH 1250
#define WINDOW_HEIGHT 750
#define TILE_SIZE 50
#define MAP_WIDTH 25
#define MAP_HEIGHT 15

int theMain();
#include "game.c"


#endif