#include <SDL2/SDL.h>
#include "pacman.h"

void initlead(){
    printf("leaderbord");
}

void initRules(){
    printf("rules");
}

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer) {
    // Load image at specified path
    SDL_Surface* loadedSurface = SDL_LoadBMP(path);
    if (!loadedSurface) {
        printf("Unable to load image %s! SDL Error: %s\n", path, SDL_GetError());
        return NULL;
    }

    // Create texture from surface pixels
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (!texture) {
        printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
    }

    // Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);

    return texture;
}

int theMain(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("Image Background", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1250, 750, SDL_WINDOW_SHOWN);
    
    if (!window) {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create renderer for window
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    // Load image
    SDL_Texture* backgroundTexture = loadTexture("Acceuil.bmp", renderer);
    if (!backgroundTexture) {
        return 1;
    }
    SDL_Rect button1 = {80, 650, 380, 90};
    SDL_Rect button2 = {310, 650, 380, 90};
    SDL_Rect button3 = {540, 650, 380, 90};
    // Main loop flag
    int quit = false;
    SDL_Event e;

    // Main loop
    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e)) {
            switch(e.type){
                case SDL_WINDOWEVENT:
                    if (e.window.event == SDL_WINDOWEVENT_CLOSE)
                        quit = true;
                        break;
                case SDL_MOUSEBUTTONDOWN: // Click de souris relâché
                    int x = 0;
                    int y = 0;
                    SDL_GetMouseState(&x, &y);
                    if(1192<x && 690<y && 1235>x && 740>y){
                        quit = true;
                        break;
                    }
                    if(434<x && 293<y && 814>x && 383>y){
                        SDL_DestroyRenderer(renderer);
                        SDL_DestroyWindow(window);
                        SDL_Quit();
                        initGame();
                        break;
                    }
                    if(434<x && 309<y && 814>x && 399>y){
                        initlead();
                        break;
                    }
                    if(434<x && 325<y && 814>x && 415>y)
                        initRules();
                        break;
            }
        }

        // Clear screen
        SDL_RenderClear(renderer);

        // Render background texture
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Free resources and close SDL
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

// SDL_Rect img = {0, 0, 1250, 750};

// SDL_Rect button4 = {1000, 650, 180, 60};
// SDL_Event events;



