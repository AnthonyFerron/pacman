#include "pacman.h"
#include <math.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* backgroundTexture = NULL;
SDL_Texture* playerTextures[4] = {NULL, NULL, NULL, NULL}; // Textures pour les 4 directions
SDL_Texture* ballTexture = NULL;
SDL_Rect playerRect;

// Directions indices
enum { UP, DOWN, LEFT, RIGHT };

//initialisation de la map en 24 par 15

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

int score = 0; // Variable globale pour le score

int isCollision(int x, int y) {
    int tileX = x / TILE_SIZE;
    int tileY = y / TILE_SIZE;
    return map[tileY][tileX] == 1;
}

void drawMap(SDL_Renderer* renderer) {
    // Load and render the background map image
    SDL_Surface* mapSurface = SDL_LoadBMP("./image/map.bmp");
    if (mapSurface == NULL) {
        printf("Failed to load map image! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    SDL_Texture* mapTexture = SDL_CreateTextureFromSurface(renderer, mapSurface);
    SDL_FreeSurface(mapSurface);
    SDL_RenderCopy(renderer, mapTexture, NULL, NULL);
    SDL_DestroyTexture(mapTexture);

    // Render the pokeballs on top of the background map
    SDL_Rect tileRect;
    tileRect.w = TILE_SIZE;
    tileRect.h = TILE_SIZE;

    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            tileRect.x = x * TILE_SIZE;
            tileRect.y = y * TILE_SIZE;
            if (map[y][x] != 1){
                if (map[y][x] != 2) {
                    // Render the texture for the pokeball
                    SDL_Rect textureRect;
                    textureRect.x = tileRect.x + (TILE_SIZE - 25) / 2;
                    textureRect.y = tileRect.y + (TILE_SIZE - 25) / 2;
                    textureRect.w = 25;
                    textureRect.h = 25;
                    SDL_RenderCopy(renderer, ballTexture, NULL, &textureRect);

                    // Increase the score when a pokeball is picked up
                    score += 10;
                }
            }
        }
    }

    // Afficher le score à l'écran
    SDL_Color textColor = {255, 255, 255}; // Couleur du texte : blanc
    char scoreText[50]; // Variable pour stocker le texte du score
    sprintf(scoreText, "Score: %d", score); // Formater le texte avec le score actuel
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText, textColor); // Créer une surface de texte à partir du texte formatté
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface); // Créer une texture à partir de la surface de texte
    SDL_Rect textRect = {10, 10, textSurface->w, textSurface->h}; // Position et taille du texte
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect); // Afficher le texte sur le rendu
    SDL_FreeSurface(textSurface); // Libérer la surface de texte
    SDL_DestroyTexture(textTexture); // Libérer la texture de texte
}

void initGame() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    // Create window
    window = SDL_CreateWindow("Pacman Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    // Load player textures for different directions
    SDL_Surface* playerSurfaces[4];
    playerSurfaces[UP] = SDL_LoadBMP("./sprites/draco_up.bmp");
    playerSurfaces[DOWN] = SDL_LoadBMP("./sprites/draco_down.bmp");
    playerSurfaces[LEFT] = SDL_LoadBMP("./sprites/draco_left.bmp");
    playerSurfaces[RIGHT] = SDL_LoadBMP("./sprites/draco_right.bmp");

    for (int i = 0; i < 4; ++i) {
        if (playerSurfaces[i] == NULL) {
            printf("Failed to load player image! SDL_Error: %s\n", SDL_GetError());
            return;
        }
        playerTextures[i] = SDL_CreateTextureFromSurface(renderer, playerSurfaces[i]);
        SDL_FreeSurface(playerSurfaces[i]);
    }

    // Load ball texture
    SDL_Surface* ballSurface = SDL_LoadBMP("./sprites/pokeball.bmp");
    if (ballSurface == NULL) {
        printf("Failed to load ball image! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    ballTexture = SDL_CreateTextureFromSurface(renderer, ballSurface);
    SDL_FreeSurface(ballSurface);

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
    int currentDirection = RIGHT; // Initial direction

    // Boucle principale
    SDL_Event event;
    int quit = 0;

    while (!quit) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        dx = 0;
                        dy = -speed;
                        currentDirection = UP;
                        break;
                    case SDLK_DOWN:
                        dx = 0;
                        dy = speed;
                        currentDirection = DOWN;
                        break;
                    case SDLK_LEFT:
                        dx = -speed;
                        dy = 0;
                        currentDirection = LEFT;
                        break;
                    case SDLK_RIGHT:
                        dx = speed;
                        dy = 0;
                        currentDirection = RIGHT;
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
            map[newY2][newX1] != 1 && map[newY2][newX2] != 1) {
            // If all the new positions are empty tiles (0), then move the player
            playerRect.x += dx;
            playerRect.y += dy;
            last_dx = dx; // Update the last valid direction
            last_dy = dy;

            // Update the map to show the player's trail
            int playerTileX1 = playerRect.x / TILE_SIZE;
            int playerTileY1 = playerRect.y / TILE_SIZE;
            int playerTileX2 = (playerRect.x + playerRect.w) / TILE_SIZE;
            int playerTileY2 = (playerRect.y + playerRect.h) / TILE_SIZE;

            // Vérifier si un des coins du joueur est entré dans une nouvelle case
            if (playerTileX1 != currentTileX || playerTileY1 != currentTileY ||
                playerTileX2 != currentTileX || playerTileY2 != currentTileY) {
                currentTileX = playerTileX1;
                currentTileY = playerTileY1;

                if (currentTileX >= 0 && currentTileX < MAP_WIDTH && 
                    currentTileY >= 0 && currentTileY < MAP_HEIGHT && 
                    map[currentTileY][currentTileX] != 2) {
                    map[currentTileY][currentTileX] = 2;
                }

                currentTileX = playerTileX2;
                currentTileY = playerTileY2;

                if (currentTileX >= 0 && currentTileX < MAP_WIDTH && 
                    currentTileY >= 0 && currentTileY < MAP_HEIGHT && 
                    map[currentTileY][currentTileX] != 2) {
                    map[currentTileY][currentTileX] = 2;
                }
            }
        } else {
            // Calculate the proposed new positions for all corners of the hitbox using the last valid direction
            newX1 = (playerRect.x + last_dx) / TILE_SIZE;
            newY1 = (playerRect.y + last_dy) / TILE_SIZE;
            newX2 = (playerRect.x + last_dx + playerRect.w) / TILE_SIZE;
            newY2 = (playerRect.y + last_dy + playerRect.h) / TILE_SIZE;

            // Check if the movement in the last valid direction would cause the player to collide with a wall
            if (newX1 >= 0 && newX1 < MAP_WIDTH && newY1 >= 0 && newY1 < MAP_HEIGHT &&
                newX2 >= 0 && newX2 < MAP_WIDTH && newY2 >= 0 && newY2 < MAP_HEIGHT &&
                map[newY1][newX1] != 1 && map[newY1][newX2] != 1 && 
                map[newY2][newX1] != 1 && map[newY2][newX2] != 1) {
                // If all the new positions are empty tiles (0), then move the player
                playerRect.x += last_dx;
                playerRect.y += last_dy;

                // Update the map to show the player's trail
                int playerTileX1 = playerRect.x / TILE_SIZE;
                int playerTileY1 = playerRect.y / TILE_SIZE;
                int playerTileX2 = (playerRect.x + playerRect.w) / TILE_SIZE;
                int playerTileY2 = (playerRect.y + playerRect.h) / TILE_SIZE;

                // Vérifier si un des coins du joueur est entré dans une nouvelle case
                if (playerTileX1 != currentTileX || playerTileY1 != currentTileY ||
                    playerTileX2 != currentTileX || playerTileY2 != currentTileY) {
                    currentTileX = playerTileX1;
                    currentTileY = playerTileY1;

                    if (currentTileX >= 0 && currentTileX < MAP_WIDTH && 
                        currentTileY >= 0 && currentTileY < MAP_HEIGHT && 
                        map[currentTileY][currentTileX] != 2) {
                        map[currentTileY][currentTileX] = 2;
                    }

                    currentTileX = playerTileX2;
                    currentTileY = playerTileY2;

                    if (currentTileX >= 0 && currentTileX < MAP_WIDTH && 
                        currentTileY >= 0 && currentTileY < MAP_HEIGHT && 
                        map[currentTileY][currentTileX] != 2) {
                        map[currentTileY][currentTileX] = 2;
                    }
                }
            }
        }

        // Clear the renderer
        SDL_RenderClear(renderer);

        // Render the background texture
        drawMap(renderer);

        // Render the player texture based on the current direction
        SDL_RenderCopy(renderer, playerTextures[currentDirection], NULL, &playerRect);

        // Update the renderer
        SDL_RenderPresent(renderer);
    }

    // Clean up resources before exiting
    for (int i = 0; i < 4; ++i) {
        if (playerTextures[i] != NULL) {
            SDL_DestroyTexture(playerTextures[i]);
            playerTextures[i] = NULL;
        }
    }
    if (ballTexture != NULL) {
        SDL_DestroyTexture(ballTexture);
        ballTexture = NULL;
    }
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    if (window != NULL) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    SDL_Quit();
}
