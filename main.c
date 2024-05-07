#include "pacman.h"

// test
int theMain() {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur lors de l'initialisation de SDL : %s\n", SDL_GetError());
        return 1;
    }

    // Création de la fenêtre
    SDL_Window* window = SDL_CreateWindow("Ma fenêtre", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (window == NULL) {
        printf("Erreur lors de la création de la fenêtre : %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Création du rendu
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    if (renderer == NULL) {
        printf("Erreur lors de la création du rendu : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    // Rectangle bleu
    SDL_Rect blueRect = { 100, 100, 200, 200 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &blueRect);

    // Rectangle rouge
    SDL_Rect redRect = { 950, 100, 200, 200 };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &redRect);

    // Affichage du rendu
    SDL_RenderPresent(renderer);

    // Boucle principale
    int quit = 0;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                if (mouseX >= redRect.x && mouseX <= redRect.x + redRect.w && mouseY >= redRect.y && mouseY <= redRect.y + redRect.h) {
                    quit = 1;
                } else if (mouseX >= blueRect.x && mouseX <= blueRect.x + blueRect.w && mouseY >= blueRect.y && mouseY <= blueRect.y + blueRect.h) {
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    initGame();
                }
            }
        }
    }

    // Libération des ressources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

int main(void) {
    theMain();
    return 0;
}