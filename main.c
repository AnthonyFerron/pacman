#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

// Fonction pour initialiser le jeu
int initGame(SDL_Renderer* renderer, SDL_Window* window) {
    // Supprimer les éléments présents sur la fenêtre
    printf("Initialisation du jeu\n");
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Charger et afficher la carte
    SDL_Surface* mapSurface = SDL_LoadBMP("Maps.bmp");
    if (mapSurface == NULL) {
        printf("Erreur lors du chargement de la carte : %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* mapTexture = SDL_CreateTextureFromSurface(renderer, mapSurface);
    if (mapTexture == NULL) {
        printf("Erreur lors de la création de la texture de la carte : %s\n", SDL_GetError());
        SDL_FreeSurface(mapSurface);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Rect mapRect = {0, 0, mapSurface->w, mapSurface->h};
    SDL_RenderCopy(renderer, mapTexture, NULL, &mapRect);

    // Charger et afficher Draco
    SDL_Surface* dracoSurface = SDL_LoadBMP("draco.bmp");
    if (dracoSurface == NULL) {
        printf("Erreur lors du chargement de Draco : %s\n", SDL_GetError());
        SDL_DestroyTexture(mapTexture);
        SDL_FreeSurface(mapSurface);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* dracoTexture = SDL_CreateTextureFromSurface(renderer, dracoSurface);
    if (dracoTexture == NULL) {
        printf("Erreur lors de la création de la texture de Draco : %s\n", SDL_GetError());
        SDL_FreeSurface(dracoSurface);
        SDL_DestroyTexture(mapTexture);
        SDL_FreeSurface(mapSurface);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Rect dracoRect = {0, 0, dracoSurface->w, dracoSurface->h};
    SDL_RenderCopy(renderer, dracoTexture, NULL, &dracoRect);

    // Afficher le rendu
    SDL_RenderPresent(renderer);
    // Création du bouton rouge
    SDL_Rect quitButton = {220, 10, 100, 50};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &quitButton);
    SDL_RenderPresent(renderer);
    // Boucle d'attente du clic sur le bouton rouge
    int redButtonClicked = 0;
    int running = 1;
    while (!redButtonClicked) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
                redButtonClicked = 1;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                if (mouseX >= 120 && mouseX <= 220 && mouseY >= 10 && mouseY <= 60) {
                    redButtonClicked = 1;
                }
            }
        }
    }
}

int main() {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur lors de l'initialisation de SDL : %s\n", SDL_GetError());
        return 1;
    }

    // Création de la fenêtre
    SDL_Window* window = SDL_CreateWindow("Ma fenêtre", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1250, 750, 0);
    if (window == NULL) {
        printf("Erreur lors de la création de la fenêtre : %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Création du rendu
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Erreur lors de la création du rendu : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Boucle principale
    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                if (mouseX >= 10 && mouseX <= 110 && mouseY >= 10 && mouseY <= 60) {
                    initGame(renderer, window);
                } else if (mouseX >= 120 && mouseX <= 220 && mouseY >= 10 && mouseY <= 60) {
                    running = 0;
                }
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Dessiner les boutons
        SDL_Rect blueButton = {10, 10, 100, 50};
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &blueButton);

        SDL_Rect redButton = {120, 10, 100, 50};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &redButton);

        // Afficher le rendu
        SDL_RenderPresent(renderer);
    }

    // Libération des ressources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}