#include "pacman.h"
#include <math.h>
#define SPEED 2

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *backgroundTexture = NULL;
SDL_Texture *playerTexture = NULL;
SDL_Texture *ballTexture = NULL;
SDL_Texture *ghost1Texture = NULL;
SDL_Texture *ghost2Texture = NULL;
SDL_Texture *ghost3Texture = NULL;
SDL_Texture *ghost4Texture = NULL;
SDL_Rect ghost1Rect, ghost2Rect, ghost3Rect, ghost4Rect;
SDL_Rect playerRect;

// initialisation de la map en 24 par 15

int map[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1},
    {0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

int isCollision(int x, int y)
{
    int tileX = x / TILE_SIZE;
    int tileY = y / TILE_SIZE;
    return map[tileY][tileX] == 1;
}

// Ajoutez cette fonction pour vérifier si deux rectangles se chevauchent
int isOverlap(SDL_Rect *rect1, SDL_Rect *rect2)
{
    if (rect1->x < rect2->x + rect2->w &&
        rect1->x + rect1->w > rect2->x &&
        rect1->y < rect2->y + rect2->h &&
        rect1->y + rect1->h > rect2->y)
    {
        return 1;
    }
    return 0;
}

// Ajoutez ces variables pour stocker la direction des fantômes
int ghost1_dx = 0, ghost1_dy = SPEED;
int ghost2_dx = 0, ghost2_dy = SPEED;
int ghost3_dx = 0, ghost3_dy = SPEED;
int ghost4_dx = 0, ghost4_dy = SPEED;

// Ajoutez cette fonction pour changer la direction d'un fantôme de manière aléatoire
void changeGhostDirection(int *dx, int *dy)
{
    int direction = rand() % 4;
    switch (direction)
    {
    case 0: // Haut
        *dx = 0;
        *dy = -SPEED;
        break;
    case 1: // Bas
        *dx = 0;
        *dy = SPEED;
        break;
    case 2: // Gauche
        *dx = -SPEED;
        *dy = 0;
        break;
    case 3: // Droite
        *dx = SPEED;
        *dy = 0;
        break;
    }
}
void drawMap(SDL_Renderer *renderer)
{
    SDL_Rect tileRect;
    tileRect.w = TILE_SIZE;
    tileRect.h = TILE_SIZE;

    // Render the background texture
    SDL_Rect backgroundRect;
    backgroundRect.x = 0;
    backgroundRect.y = 0;
    backgroundRect.w = WINDOW_WIDTH;
    backgroundRect.h = WINDOW_HEIGHT;
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

    for (int y = 0; y < MAP_HEIGHT; ++y)
    {
        for (int x = 0; x < MAP_WIDTH; ++x)
        {
            tileRect.x = x * TILE_SIZE;
            tileRect.y = y * TILE_SIZE;

            if (map[y][x] == 0)
            {
                // Render the texture for the pokeball
                SDL_Rect textureRect;
                textureRect.x = tileRect.x + (TILE_SIZE - 25) / 2;
                textureRect.y = tileRect.y + (TILE_SIZE - 25) / 2;
                textureRect.w = 25;
                textureRect.h = 25;
                SDL_RenderCopy(renderer, ballTexture, NULL, &textureRect);
            }
        }
    }
}

void initGame()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    // Create window
    window = SDL_CreateWindow("Pacman Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    // Load map BG

    SDL_Surface *backgroundSurface = SDL_LoadBMP("./image/Map1.bmp");
    if (backgroundSurface == NULL)
    {
        printf("Failed to load map image! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    // Load player texture
    SDL_Surface *playerSurface = SDL_LoadBMP("./sprites/draco.bmp");
    if (playerSurface == NULL)
    {
        printf("Failed to load player image! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
    SDL_FreeSurface(playerSurface);

    // Load ball texture
    SDL_Surface *ballSurface = SDL_LoadBMP("./sprites/pokeball.bmp");
    if (ballSurface == NULL)
    {
        printf("Failed to load player image! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    ballTexture = SDL_CreateTextureFromSurface(renderer, ballSurface);
    SDL_FreeSurface(ballSurface);

    // Dans votre fonction initGame(), ajoutez ces lignes pour charger les textures des fantômes
    SDL_Surface *ghost1Surface = SDL_LoadBMP("./sprites/ghost1.bmp");
    ghost1Texture = SDL_CreateTextureFromSurface(renderer, ghost1Surface);
    SDL_FreeSurface(ghost1Surface);

    SDL_Surface *ghost2Surface = SDL_LoadBMP("./sprites/ghost2.bmp");
    ghost2Texture = SDL_CreateTextureFromSurface(renderer, ghost2Surface);
    SDL_FreeSurface(ghost2Surface);

    SDL_Surface *ghost3Surface = SDL_LoadBMP("./sprites/ghost3.bmp");
    ghost3Texture = SDL_CreateTextureFromSurface(renderer, ghost3Surface);
    SDL_FreeSurface(ghost3Surface);

    SDL_Surface *ghost4Surface = SDL_LoadBMP("./sprites/ghost4.bmp");
    ghost4Texture = SDL_CreateTextureFromSurface(renderer, ghost4Surface);
    SDL_FreeSurface(ghost4Surface);

    // Définissez les positions initiales des fantômes
    ghost1Rect.x = 300;
    ghost1Rect.y = 300;
    ghost1Rect.w = TILE_SIZE - 1;
    ghost1Rect.h = TILE_SIZE - 1;
    ghost2Rect.x = 950;
    ghost2Rect.y = 200;
    ghost2Rect.w = TILE_SIZE - 1;
    ghost2Rect.h = TILE_SIZE - 1;
    ghost3Rect.x = 950;
    ghost3Rect.y = 500;
    ghost3Rect.w = TILE_SIZE - 1;
    ghost3Rect.h = TILE_SIZE - 1;
    ghost4Rect.x = 500;
    ghost4Rect.y = 500;
    ghost4Rect.w = TILE_SIZE - 1;
    ghost4Rect.h = TILE_SIZE - 1;

    // Changez la direction des fantômes de manière aléatoire à chaque tour
    changeGhostDirection(&ghost1_dx, &ghost1_dy);
    changeGhostDirection(&ghost2_dx, &ghost2_dy);
    changeGhostDirection(&ghost3_dx, &ghost3_dy);
    changeGhostDirection(&ghost4_dx, &ghost4_dy);

    // Set initial position of player
    playerRect.x = 650;
    playerRect.y = 350;
    playerRect.w = TILE_SIZE - 1;
    playerRect.h = TILE_SIZE - 1;

    // Variables to store the player's current tile
    int currentTileX = playerRect.x / TILE_SIZE;
    int currentTileY = playerRect.y / TILE_SIZE;

    // Définir la vitesse et la direction du personnage
    int speed = 10;
    int dx = 0, dy = 0;
    int last_dx = 0, last_dy = 0; // Ajouter ces lignes pour stocker la dernière direction valide

    // Boucle principale
    SDL_Event event;
    int quit = 0;

    while (!quit)
    {

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = 1;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    dx = 0;
                    dy = -speed;
                    break;
                case SDLK_DOWN:
                    dx = 0;
                    dy = speed;
                    break;
                case SDLK_LEFT:
                    dx = -speed;
                    dy = 0;
                    break;
                case SDLK_RIGHT:
                    dx = speed;
                    dy = 0;
                    break;
                }
            }
        }

        // Calculer les nouvelles positions proposées pour tous les coins de la hitbox
        int newX1 = (playerRect.x + dx) / TILE_SIZE;
        int newY1 = (playerRect.y + dy) / TILE_SIZE;
        int newX2 = (playerRect.x + dx + playerRect.w) / TILE_SIZE;
        int newY2 = (playerRect.y + dy + playerRect.h) / TILE_SIZE;

        // Vérifier si le mouvement proposé ferait entrer player en collision avec un mur
        if (newX1 >= 0 && newX1 < MAP_WIDTH && newY1 >= 0 && newY1 < MAP_HEIGHT &&
            newX2 >= 0 && newX2 < MAP_WIDTH && newY2 >= 0 && newY2 < MAP_HEIGHT &&
            map[newY1][newX1] != 1 && map[newY1][newX2] != 1 &&
            map[newY2][newX1] != 1 && map[newY2][newX2] != 1)
        {
            // If all the new positions are empty tiles (0), then move the player
            playerRect.x += dx;
            playerRect.y += dy;
            last_dx = dx; // Update the last valid direction
            last_dy = dy;

            // Mettre à jour la position du fantôme 1
            ghost1Rect.x += ghost1_dx;
            ghost1Rect.y += ghost1_dy;
            ghost2Rect.x += ghost2_dx;
            ghost2Rect.y += ghost2_dy;
            ghost3Rect.x += ghost3_dx;
            ghost3Rect.y += ghost3_dy;
            ghost4Rect.x += ghost4_dx;
            ghost4Rect.y += ghost4_dy;

            // Calculer les nouvelles positions proposées pour tous les coins de la hitbox du fantôme
            int newGhost1X1 = (ghost1Rect.x + ghost1_dx) / TILE_SIZE;
            int newGhost1Y1 = (ghost1Rect.y + ghost1_dy) / TILE_SIZE;
            int newGhost1X2 = (ghost1Rect.x + ghost1_dx + ghost1Rect.w) / TILE_SIZE;
            int newGhost1Y2 = (ghost1Rect.y + ghost1_dy + ghost1Rect.h) / TILE_SIZE;
            int newGhost2X1 = (ghost2Rect.x + ghost2_dx) / TILE_SIZE;
            int newGhost2Y1 = (ghost2Rect.y + ghost2_dy) / TILE_SIZE;
            int newGhost2X2 = (ghost2Rect.x + ghost2_dx + ghost2Rect.w) / TILE_SIZE;
            int newGhost2Y2 = (ghost2Rect.y + ghost2_dy + ghost2Rect.h) / TILE_SIZE;
            int newGhost3X1 = (ghost3Rect.x + ghost3_dx) / TILE_SIZE;
            int newGhost3Y1 = (ghost3Rect.y + ghost3_dy) / TILE_SIZE;
            int newGhost3X2 = (ghost3Rect.x + ghost3_dx + ghost3Rect.w) / TILE_SIZE;
            int newGhost3Y2 = (ghost3Rect.y + ghost3_dy + ghost3Rect.h) / TILE_SIZE;
            int newGhost4X1 = (ghost4Rect.x + ghost4_dx) / TILE_SIZE;
            int newGhost4Y1 = (ghost4Rect.y + ghost4_dy) / TILE_SIZE;
            int newGhost4X2 = (ghost4Rect.x + ghost4_dx + ghost4Rect.w) / TILE_SIZE;
            int newGhost4Y2 = (ghost4Rect.y + ghost4_dy + ghost4Rect.h) / TILE_SIZE;

            // Vérifier si le mouvement proposé ferait entrer le fantôme 1 en collision avec un mur
            if (newGhost1X1 >= 0 && newGhost1X1 < MAP_WIDTH && newGhost1Y1 >= 0 && newGhost1Y1 < MAP_HEIGHT &&
                newGhost1X2 >= 0 && newGhost1X2 < MAP_WIDTH && newGhost1Y2 >= 0 && newGhost1Y2 < MAP_HEIGHT &&
                map[newGhost1Y1][newGhost1X1] != 1 && map[newGhost1Y1][newGhost1X2] != 1 &&
                map[newGhost1Y2][newGhost1X1] != 1 && map[newGhost1Y2][newGhost1X2] != 1)
            {
                // Si toutes les nouvelles positions sont des tuiles vides (0), alors déplacer le fantôme
                ghost1Rect.x += ghost1_dx;
                ghost1Rect.y += ghost1_dy;
            }
            else
            {
                // Si une collision se produit, changer la direction du fantôme
                changeGhostDirection(&ghost1_dx, &ghost1_dy);
            }

            // Vérifier si le mouvement proposé ferait entrer le fantôme 2 en collision avec un mur
            if (newGhost2X1 >= 0 && newGhost2X1 < MAP_WIDTH && newGhost2Y1 >= 0 && newGhost2Y1 < MAP_HEIGHT &&
                newGhost2X2 >= 0 && newGhost2X2 < MAP_WIDTH && newGhost2Y2 >= 0 && newGhost2Y2 < MAP_HEIGHT &&
                map[newGhost2Y1][newGhost2X1] != 1 && map[newGhost2Y1][newGhost2X2] != 1 &&
                map[newGhost2Y2][newGhost2X1] != 1 && map[newGhost2Y2][newGhost2X2] != 1)
            {
                // Si toutes les nouvelles positions sont des tuiles vides (0), alors déplacer le fantôme
                ghost2Rect.x += ghost2_dx;
                ghost2Rect.y += ghost2_dy;
            }
            else
            {
                // Si une collision se produit, changer la direction du fantôme
                changeGhostDirection(&ghost2_dx, &ghost2_dy);
            }

            // Vérifier si le mouvement proposé ferait entrer le fantôme 3 en collision avec un mur
            if (newGhost3X1 >= 0 && newGhost3X1 < MAP_WIDTH && newGhost3Y1 >= 0 && newGhost3Y1 < MAP_HEIGHT &&
                newGhost3X2 >= 0 && newGhost3X2 < MAP_WIDTH && newGhost3Y2 >= 0 && newGhost3Y2 < MAP_HEIGHT &&
                map[newGhost3Y1][newGhost3X1] != 1 && map[newGhost3Y1][newGhost3X2] != 1 &&
                map[newGhost3Y2][newGhost3X1] != 1 && map[newGhost3Y2][newGhost3X2] != 1)
            {
                // Si toutes les nouvelles positions sont des tuiles vides (0), alors déplacer le fantôme
                ghost3Rect.x += ghost3_dx;
                ghost3Rect.y += ghost3_dy;
            }
            else
            {
                // Si une collision se produit, changer la direction du fantôme
                changeGhostDirection(&ghost3_dx, &ghost3_dy);
            }
            // Vérifier si le mouvement proposé ferait entrer le fantôme 4 en collision avec un mur
            if (newGhost4X1 >= 0 && newGhost4X1 < MAP_WIDTH && newGhost4Y1 >= 0 && newGhost4Y1 < MAP_HEIGHT &&
                newGhost4X2 >= 0 && newGhost4X2 < MAP_WIDTH && newGhost4Y2 >= 0 && newGhost4Y2 < MAP_HEIGHT &&
                map[newGhost4Y1][newGhost4X1] != 1 && map[newGhost4Y1][newGhost4X2] != 1 &&
                map[newGhost4Y2][newGhost4X1] != 1 && map[newGhost4Y2][newGhost4X2] != 1)
            {
                // Si toutes les nouvelles positions sont des tuiles vides (0), alors déplacer le fantôme
                ghost4Rect.x += ghost4_dx;
                ghost4Rect.y += ghost4_dy;
            }
            else
            {
                // Si une collision se produit, changer la direction du fantôme
                changeGhostDirection(&ghost4_dx, &ghost4_dy);
            }

            // Update the map to show the player's trail
            int playerTileX = playerRect.x / TILE_SIZE;
            int playerTileY = playerRect.y / TILE_SIZE;
            if (playerTileX != currentTileX || playerTileY != currentTileY)
            {
                // The player has moved to a new tile, so update the map and redraw
                if (playerTileX >= 0 && playerTileX < MAP_WIDTH && playerTileY >= 0 && playerTileY < MAP_HEIGHT && map[playerTileY][playerTileX] != 2)
                {
                    map[playerTileY][playerTileX] = 2;
                }
                currentTileX = playerTileX;
                currentTileY = playerTileY;
            }
            // Dans votre boucle principale, vérifiez si le joueur a heurté un fantôme
            if (isOverlap(&playerRect, &ghost1Rect) ||
                isOverlap(&playerRect, &ghost2Rect) ||
                isOverlap(&playerRect, &ghost3Rect) ||
                isOverlap(&playerRect, &ghost4Rect))
            {
                quit = 1; // Fermez le jeu si le joueur a heurté un fantôme
            }
        }
        else
        {
            // Calculate the proposed new positions for all corners of the hitbox using the last valid direction
            newX1 = (playerRect.x + last_dx) / TILE_SIZE;
            newY1 = (playerRect.y + last_dy) / TILE_SIZE;
            newX2 = (playerRect.x + last_dx + playerRect.w) / TILE_SIZE;
            newY2 = (playerRect.y + last_dy + playerRect.h) / TILE_SIZE;

            // Check if the movement in the last valid direction would cause the player to collide with a wall
            if (newX1 >= 0 && newX1 < MAP_WIDTH && newY1 >= 0 && newY1 < MAP_HEIGHT &&
                newX2 >= 0 && newX2 < MAP_WIDTH && newY2 >= 0 && newY2 < MAP_HEIGHT &&
                map[newY1][newX1] != 1 && map[newY1][newX2] != 1 &&
                map[newY2][newX1] != 1 && map[newY2][newX2] != 1)
            {
                // If all the new positions are empty tiles (0), then move the player
                playerRect.x += last_dx;
                playerRect.y += last_dy;
            }
            // If a collision occurs, do not move the player
        }

        // Clear the renderer
        SDL_RenderClear(renderer);

        // Render the background texture
        SDL_Rect backgroundRect;
        backgroundRect.x = 0;
        backgroundRect.y = 0;
        backgroundRect.w = WINDOW_WIDTH;
        backgroundRect.h = WINDOW_HEIGHT;
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        // Render the background texture
        drawMap(renderer);

        // Render the player texture
        SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);
        // Ajoutez ces lignes pour rendre les textures des fantômes
        SDL_RenderCopy(renderer, ghost1Texture, NULL, &ghost1Rect);
        SDL_RenderCopy(renderer, ghost2Texture, NULL, &ghost2Rect);
        SDL_RenderCopy(renderer, ghost3Texture, NULL, &ghost3Rect);
        SDL_RenderCopy(renderer, ghost4Texture, NULL, &ghost4Rect);

        // Update the renderer
        SDL_RenderPresent(renderer);
    }
}
