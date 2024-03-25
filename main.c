#include <SDL2/SDL.h>
#include "pacman.h"

void initGame(){
    printf("jeu");
}


int setWindowColor(SDL_Renderer *renderer, SDL_Color color)
{
    if(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
        return -1;
    if(SDL_RenderClear(renderer) < 0)
        return -1;
    return 0;  
}

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event events;
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color red = {255, 0, 0, 255};
    SDL_Color green = {68, 255, 0, 255};
    SDL_Point point[640];
    int statut = EXIT_FAILURE;
    bool quit = false;

    
    /* Initialisation, création de la fenêtre et du renderer. */
    if(0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }
    window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              1250, 750, SDL_WINDOW_SHOWN);
    if(NULL == window)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        goto Quit;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(NULL == renderer)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        goto Quit;
    }
    SDL_Rect button1 = {80, 650, 180, 60};
    SDL_Rect button2 = {310, 650, 180, 60};
    SDL_Rect button3 = {540, 650, 180, 60};
    SDL_Rect button4 = {770, 650, 180, 60};
    SDL_Rect button5 = {1000, 650, 180, 60};

    while( !quit ) {
        while (SDL_PollEvent(&events)) {
            switch(events.type){
                case SDL_WINDOWEVENT:
                    if (events.window.event == SDL_WINDOWEVENT_CLOSE)
                        quit = true;
                        break;
                case SDL_KEYDOWN:
                    SDL_Log("+key");

                    if (events.key.keysym.scancode == SDL_SCANCODE_W)
                        SDL_Log("Scancode W");

                    if (events.key.keysym.sym == SDLK_w)
                        SDL_Log("Keycode W");

                    if (events.key.keysym.sym == SDLK_z)
                        SDL_Log("Keycode Z");

                    break;
                case SDL_MOUSEBUTTONDOWN: // Click de souris relâché
                    int x = 0;
                    int y = 0;
                    SDL_GetMouseState(&x, &y);
                    if(button5.x<x && button5.y<y && (button5.w+button5.x)>x && (button5.h+button5.y)>y)
                        quit = true;
                        break;
                    if(button1.x<x && button1.y<y && 260>x && 710>y)
                        initGame();
                        break;
            }
        }
        setWindowColor(renderer, white);
        SDL_SetRenderDrawColor(renderer, green.r, green.g, green.b, green.a);
        SDL_RenderFillRect(renderer, &button1); 
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &button2); 
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &button3); 
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &button4);
        SDL_SetRenderDrawColor(renderer, red.r, red.g, red.b, red.a);
        SDL_RenderFillRect(renderer, &button5);  
        SDL_RenderPresent(renderer);
        statut = EXIT_SUCCESS;
    }

Quit:
    if(NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if(NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;
}