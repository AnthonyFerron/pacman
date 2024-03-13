#include "pacman.h"



SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* backgroundTexture = NULL;
SDL_Texture* dracoTexture = NULL;
SDL_Rect dracoRect;


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

    // Load background texture
    SDL_Surface* backgroundSurface = SDL_LoadBMP("Maps.bmp");
    if (backgroundSurface == NULL) {
        printf("Failed to load background image! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    // Load draco texture
    SDL_Surface* dracoSurface = SDL_LoadBMP("draco.bmp");
    if (dracoSurface == NULL) {
        printf("Failed to load draco image! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    dracoTexture = SDL_CreateTextureFromSurface(renderer, dracoSurface);
    SDL_FreeSurface(dracoSurface);

    // Set initial position of draco
    dracoRect.x = (WINDOW_WIDTH - dracoSurface->w) / 2;
    dracoRect.y = (WINDOW_HEIGHT - dracoSurface->h) / 2;
    dracoRect.w = dracoSurface->w;
    dracoRect.h = dracoSurface->h;
    dracoRect.w *= 2;
    dracoRect.h *= 2;

    // Définir la vitesse et la direction du personnage
    int speed = 1;
    int dx = 0, dy = 0;
    int frameCounter = 0;

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

        // Vérifier si le nouveau mouvement ferait sortir le personnage de l'écran
        if (dracoRect.x + dx >= 0 && dracoRect.x + dx + dracoRect.w <= WINDOW_WIDTH &&
            dracoRect.y + dy >= 0 && dracoRect.y + dy + dracoRect.h <= WINDOW_HEIGHT) {
            // Mettre à jour la position du personnage seulement tous les 2 frames
            if (frameCounter % 2 == 0) {
                dracoRect.x += dx;
                dracoRect.y += dy;
            }
        }
        

        frameCounter++;

        // Clear the renderer
        SDL_RenderClear(renderer);

        // Render the background texture
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

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