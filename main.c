#include <SDL2/SDL.h>


void drawQuitButton(SDL_Renderer *renderer, SDL_Rect *quitButton, SDL_Rect *changeColor)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
    SDL_RenderFillRect(renderer, quitButton);
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // blue color
    SDL_RenderFillRect(renderer, changeColor);
    SDL_RenderPresent(renderer);
    return;
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    // Initialize SDL windows 1000x1000 pixels that last 3000ms
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("SDL2 Displaying Image",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1250, 750, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface *image = SDL_LoadBMP("hello.bmp");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Draw the quit button
    SDL_Rect quitButton = {1000, 20, 100, 50};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
    SDL_RenderFillRect(renderer, &quitButton);
    SDL_Rect changeColor = {100, 20, 100, 50};
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // blue color
    SDL_RenderFillRect(renderer, &changeColor);

    SDL_RenderPresent(renderer);


    // Event loop
    SDL_Event event;
    int running = 1;
    int color = 0;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                // Check if mouse is in the area of the quit button
                if (x > quitButton.x && x < quitButton.x + quitButton.w && y > quitButton.y && y < quitButton.y + quitButton.h) {
                    running = 0;
                }
            }
            if(event.type == SDL_MOUSEBUTTONDOWN){
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x > changeColor.x && x < changeColor.x + changeColor.w && y > changeColor.y && y < changeColor.y + changeColor.h) {
                    if(color == 0){
                        color = 1;
                        SDL_RenderClear(renderer);
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // white color
                        SDL_RenderPresent(renderer);
                        drawQuitButton(renderer, &quitButton, &changeColor);
                    }
                    else{
                        color = 0;
                        SDL_RenderClear(renderer);
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // white color
                        SDL_RenderPresent(renderer);
                        drawQuitButton(renderer, &quitButton, &changeColor);
                    }

                }
            }
        }
        SDL_Delay(100);
    }

    // Clean up
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}