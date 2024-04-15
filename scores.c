#include "SDL.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void initScores(void)
{
    char name[21];

    // LECTURE DU FICHIER TXT

    FILE *file = fopen("scores.txt", "r");
    if (file == NULL)
    {
        printf("Impossible d'ouvrir le fichier.\n");
        return;
    }

    if (fscanf(file, "%20s", name) != 1)
    {
        printf("Erreur lors de la lecture du fichier.\n");
        fclose(file);
        return;
    }

    fclose(file);

    printf("Le nom enregistré est : %s\n", name);

    // Affichage des lettres du fichier txt dans une fenetre
    SDL_Window *window = NULL;
    SDL_Surface *screenSurface = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1250, 750, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            screenSurface = SDL_GetWindowSurface(window);

            // Load the background image
            SDL_Surface *bgImage = SDL_LoadBMP("bgScores.bmp");
            if (bgImage == NULL)
            {
                printf("Unable to load image bgScores.bmp! SDL Error: %s\n", SDL_GetError());
            }
            else
            {
                // Draw the background on the screen
                SDL_BlitSurface(bgImage, NULL, screenSurface, NULL);
                SDL_FreeSurface(bgImage);
            }

            for (int j = 0; name[j] != '\0'; ++j)
            {
                char filename[128];
                sprintf(filename, "lettres/%c.bmp", toupper(name[j]));
                SDL_Surface *letterImage = SDL_LoadBMP(filename);
                if (letterImage == NULL)
                {
                    printf("Unable to load image %s! SDL Error: %s\n", filename, SDL_GetError());
                    continue;
                }

                SDL_Rect dest;
                dest.x = j * letterImage->w; // Change this to position the images side by side
                dest.y = 0;                  // Change this to position the names on different lines
                SDL_BlitSurface(letterImage, NULL, screenSurface, &dest);
                SDL_FreeSurface(letterImage);
            }

            SDL_UpdateWindowSurface(window);

            SDL_Event e;
            bool quit = false;
            while (!quit)
            {
                while (SDL_PollEvent(&e) != 0)
                {
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                }
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}

void saveScores(void)
{
    char name[21];

    // OUVERTURE DU FICHIER TXT

    FILE *file = fopen("scores.txt", "w");
    if (file == NULL)
    {
        printf("Impossible d'ouvrir le fichier.\n");
        return;
    }

    printf("Veuillez entrer un nom à enregistrer :\n");
    scanf("%20s", name);

    fprintf(file, "%s\n", name);

    fclose(file);

    printf("Le nom '%s' a été enregistré.\n", name);

    SDL_DestroyWindow(window);
    SDL_Quit();
}
