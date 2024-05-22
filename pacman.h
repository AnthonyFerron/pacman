#ifndef PACMAN
#define PACMAN

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL.h>
#include <stdbool.h>
#include <time.h>

#define WINDOW_WIDTH 1350
#define WINDOW_HEIGHT 850
#define TILE_SIZE 50
#define MAP_WIDTH 27
#define MAP_HEIGHT 17
int gameScore = 0;

int theMain();
#include "game.c"
#include "afficherScores.c"
#include "saveScores.c"

#endif