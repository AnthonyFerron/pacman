#include "SDL.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "saveScores.h"

typedef struct
{
    char name[21];
    int score;
} ScoreEntry;

void saveScores(int score)
{
    ScoreEntry scores[3];
    ScoreEntry newEntry;
    char input[21] = "";
    bool enterName = true;

    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Erreur lors de l'initialisation de SDL: %s\n", SDL_GetError());
        return;
    }

    // Création de la fenêtre
    SDL_Window *window = SDL_CreateWindow("Entrée du score", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1250, 750, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Erreur lors de la création de la fenêtre: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    // Création du rendu
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Erreur lors de la création du rendu: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Chargement des images des lettres et des chiffres
    SDL_Surface *letters[36];
    for (int i = 0; i < 36; ++i)
    {
        char filename[30];
        sprintf(filename, "lettres/%c.bmp", i < 26 ? 'a' + i : '0' + i - 26);
        letters[i] = SDL_LoadBMP(filename);
        if (letters[i] == NULL)
        {
            printf("Erreur lors du chargement de l'image %s: %s\n", filename, SDL_GetError());
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return;
        }
    }

    // Boucle principale
    bool running = true;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_TEXTINPUT)
            {
                // Ajouter le texte entré à la chaîne d'entrée
                strncat(input, event.text.text, 20 - strlen(input));
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_RETURN)
                {
                    // Si la touche Entrée est pressée, passer à l'entrée du score
                    if (enterName)
                    {
                        strncpy(newEntry.name, input, 21);
                        enterName = false;
                    }
                    else
                    {
                        newEntry.score = newEntry.score = score; // Utilisez le score passé en argument;
                        running = false;
                    }
                    memset(input, 0, 21);
                }
                else if (event.key.keysym.sym == SDLK_BACKSPACE)
                {
                    // Si la touche de retour arrière est pressée, supprimer le dernier caractère
                    if (strlen(input) > 0)
                    {
                        input[strlen(input) - 1] = '\0';
                    }
                }
            }
        }

        // Rendu
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        for (int i = 0; i < strlen(input); ++i)
        {
            int index;
            if (input[i] >= 'a' && input[i] <= 'z')
            {
                index = input[i] - 'a';
            }
            else if (input[i] >= '0' && input[i] <= '9')
            {
                index = input[i] - '0' + 26;
            }
            else
            {
                continue;
            }
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, letters[index]);
            SDL_Rect rect = {i * 20, 0, 20, 20};
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            SDL_DestroyTexture(texture);
        }
        SDL_RenderPresent(renderer);

        SDL_Delay(10);
    }

    // Libération des images des lettres et des chiffres
    for (int i = 0; i < 36; ++i)
    {
        SDL_FreeSurface(letters[i]);
    }

    // OUVERTURE DU FICHIER TXT

    FILE *file = fopen("scores.txt", "r");
    if (file == NULL)
    {
        printf("Impossible d'ouvrir le fichier.\n");
        return;
    }

    for (int i = 0; i < 3; ++i)
    {
        if (fscanf(file, "%20s %d", scores[i].name, &scores[i].score) != 2)
        {
            printf("Erreur lors de la lecture du fichier.\n");
            fclose(file);
            return;
        }
    }
    fclose(file);

    // Trouver l'entrée avec le score le plus bas
    int minIndex = 0;
    for (int i = 1; i < 3; ++i)
    {
        if (scores[i].score < scores[minIndex].score)
        {
            minIndex = i;
        }
    }

    // Si le nouveau score est plus élevé que le score le plus bas, remplacer l'entrée
    if (newEntry.score > scores[minIndex].score)
    {
        scores[minIndex] = newEntry;
    }

    // Trier les scores dans l'ordre décroissant
    for (int i = 0; i < 3; ++i)
    {
        for (int j = i + 1; j < 3; ++j)
        {
            if (scores[j].score > scores[i].score)
            {
                ScoreEntry temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }
    }

    // Écrire les nouveaux scores dans le fichier
    file = fopen("scores.txt", "w");
    if (file == NULL)
    {
        printf("Impossible d'ouvrir le fichier.\n");
        return;
    }
    for (int i = 0; i < 3; ++i)
    {
        fprintf(file, "%s %d\n", scores[i].name, scores[i].score);
    }
    fclose(file);

    printf("Le nom '%s' et le score '%d' ont été enregistrés.\n", newEntry.name, newEntry.score);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    initScores();
}