#include "pacman.h"



SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* backgroundTexture = NULL;
SDL_Texture* dracoTexture = NULL;
SDL_Rect dracoRect;

int map[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1},
    {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

int isCollision(int x, int y) {
    int tileX = x / TILE_SIZE;
    int tileY = y / TILE_SIZE;
    return map[tileY][tileX] == 1;
}

void drawMap(SDL_Renderer* renderer) {
    SDL_Rect tileRect;
    tileRect.w = TILE_SIZE;
    tileRect.h = TILE_SIZE;

    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {

            tileRect.x = x * TILE_SIZE;
            tileRect.y = y * TILE_SIZE;

            if (map[y][x] == 1) {
                // Draw wall
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color
            } else {
                // Draw empty space
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color
            }

            SDL_RenderFillRect(renderer, &tileRect);
        }
    }
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

    // // Load background texture
    // SDL_Surface* backgroundSurface = SDL_LoadBMP("Maps.bmp");
    // if (backgroundSurface == NULL) {
    //     printf("Failed to load background image! SDL_Error: %s\n", SDL_GetError());
    //     return;
    // }
    // backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    // SDL_FreeSurface(backgroundSurface);


    // Load draco texture
    SDL_Surface* dracoSurface = SDL_LoadBMP("draco.bmp");
    if (dracoSurface == NULL) {
        printf("Failed to load draco image! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    dracoTexture = SDL_CreateTextureFromSurface(renderer, dracoSurface);
    SDL_FreeSurface(dracoSurface);

    // Set initial position of draco
    dracoRect.x = 400;
    dracoRect.y = 600;
    dracoRect.w = dracoSurface->w;
    dracoRect.h = dracoSurface->h;
    dracoRect.w = 99;
    dracoRect.h = 99;

    // Définir la vitesse et la direction du personnage
    int speed = 1;
    int dx = 0, dy = 0;
    int last_dx = 0, last_dy = 0; // Ajouter ces lignes pour stocker la dernière direction valide

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
        int newX1 = (dracoRect.x + dx) / TILE_SIZE;
        int newY1 = (dracoRect.y + dy) / TILE_SIZE;
        int newX2 = (dracoRect.x + dx + dracoRect.w) / TILE_SIZE;
        int newY2 = (dracoRect.y + dy + dracoRect.h) / TILE_SIZE;

        // Vérifier si le mouvement proposé ferait entrer Draco en collision avec un mur
        if (map[newY1][newX1] == 0 && map[newY1][newX2] == 0 && map[newY2][newX1] == 0 && map[newY2][newX2] == 0) {
            // Si toutes les nouvelles positions sont des cases vides (0), alors déplacer Draco
            dracoRect.x += dx;
            dracoRect.y += dy;
            last_dx = dx; // Mettre à jour la dernière direction valide
            last_dy = dy;
        } else {
            // Calculer les nouvelles positions proposées pour tous les coins de la hitbox en utilisant la dernière direction valide
            newX1 = (dracoRect.x + last_dx) / TILE_SIZE;
            newY1 = (dracoRect.y + last_dy) / TILE_SIZE;
            newX2 = (dracoRect.x + last_dx + dracoRect.w) / TILE_SIZE;
            newY2 = (dracoRect.y + last_dy + dracoRect.h) / TILE_SIZE;

            // Vérifier si le mouvement dans la dernière direction valide ferait entrer Draco en collision avec un mur
            if (map[newY1][newX1] == 0 && map[newY1][newX2] == 0 && map[newY2][newX1] == 0 && map[newY2][newX2] == 0) {
                // Si toutes les nouvelles positions sont des cases vides (0), alors déplacer Draco
                dracoRect.x += last_dx;
                dracoRect.y += last_dy;
            }
            // Si une collision se produit, ne pas déplacer Draco
        }
    
        


        // Clear the renderer
        SDL_RenderClear(renderer);

        // Render the background texture
        // SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        drawMap(renderer);

        // Render the draco texture
        SDL_RenderCopy(renderer, dracoTexture, NULL, &dracoRect);

        // Update the renderer
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(dracoTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}