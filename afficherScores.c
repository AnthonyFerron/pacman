#include "pacman.h"

void initScores(void)
{
    char names[3][21];
    int scores[3];

    // LECTURE DU FICHIER TXT

    FILE *file = fopen("scores.txt", "r");
    if (file == NULL)
    {
        printf("Impossible d'ouvrir le fichier.\n");
        return;
    }

    for (int i = 0; i < 3; ++i)
    {
        if (fscanf(file, "%20s %d", names[i], &scores[i]) != 2)
        {
            printf("Erreur lors de la lecture du fichier.\n");
            fclose(file);
            return;
        }
    }

    fclose(file);

    // Affichage des lettres du fichier txt dans une fenetre
    SDL_Window *window = NULL;
    SDL_Surface *screenSurface = NULL;
    SDL_Rect buttonRect; // Déplacez la déclaration ici

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
            SDL_Surface *bgImage = SDL_LoadBMP("./image/bgScores.bmp");
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

            // Largeur et hauteur de la fenêtre
            int windowWidth = 1250;
            int windowHeight = 750;

            for (int i = 0; i < 3; ++i)
            {
                // Nombre de lettres dans le nom
                int nameLength = strlen(names[i]);

                for (int j = 0; names[i][j] != '\0'; ++j)
                {
                    char filename[128];
                    sprintf(filename, "lettres/%c.bmp", toupper(names[i][j]));
                    SDL_Surface *letterImage = SDL_LoadBMP(filename);
                    if (letterImage == NULL)
                    {
                        printf("Unable to load image %s! SDL Error: %s\n", filename, SDL_GetError());
                        continue;
                    }

                    // Largeur et hauteur de l'image de la lettre
                    int letterWidth = letterImage->w;
                    int letterHeight = letterImage->h;

                    // Largeur totale des lettres
                    int totalWidth = nameLength * letterWidth;

                    // Position de départ pour centrer les lettres
                    int startX = ((windowWidth - totalWidth) / 2) - 75;
                    int startY = ((windowHeight - letterHeight) / 2) + i * 100; // Change this line to increase the vertical margin

                    SDL_Rect dest;
                    dest.x = startX + j * (letterWidth - 20); // Change this to position the images side by side
                    dest.y = startY;                          // Change this to position the names on different lines
                    SDL_BlitSurface(letterImage, NULL, screenSurface, &dest);
                    SDL_FreeSurface(letterImage);
                }

                // Affichage du score
                char scoreStr[11]; // Un entier peut avoir jusqu'à 10 chiffres
                sprintf(scoreStr, "%d", scores[i]);
                int scoreLength = strlen(scoreStr);

                for (int j = 0; scoreStr[j] != '\0'; ++j)
                {
                    char filename[128];
                    sprintf(filename, "lettres/%c.bmp", scoreStr[j]);
                    SDL_Surface *scoreImage = SDL_LoadBMP(filename);
                    if (scoreImage == NULL)
                    {
                        printf("Unable to load image %s! SDL Error: %s\n", filename, SDL_GetError());
                        continue;
                    }

                    // Largeur et hauteur de l'image du score
                    int scoreWidth = scoreImage->w;
                    int scoreHeight = scoreImage->h;

                    // Largeur totale des chiffres du score
                    int totalScoreWidth = scoreLength * scoreWidth;

                    // Position de départ pour centrer les chiffres du score
                    int startX = ((windowWidth - totalScoreWidth) / 2) - 100 + (nameLength * (scoreWidth - 20)); // Change this line to position the scores next to the names
                    int startY = ((windowHeight - scoreHeight) / 2) + i * 100;                                   // Change this line to position the scores on the same line as the names

                    SDL_Rect dest;
                    dest.x = startX + j * (scoreWidth - 20); // Change this to position the images side by side
                    dest.y = startY;                         // Change this to position the scores on different lines
                    SDL_BlitSurface(scoreImage, NULL, screenSurface, &dest);
                    SDL_FreeSurface(scoreImage);
                }
            }

            // Charger l'image du bouton
            SDL_Surface *buttonImage = SDL_LoadBMP("./image/back.bmp");
            if (buttonImage == NULL)
            {
                printf("Impossible de charger l'image du bouton! SDL Error: %s\n", SDL_GetError());
            }
            else
            {
                // Définir la position du bouton
                buttonRect.x = 10;  // Positionne le bouton à droite
                buttonRect.y = windowHeight - (buttonImage->h + 10); // Positionne le bouton en bas
                buttonRect.w = buttonImage->w;                // Largeur du bouton
                buttonRect.h = buttonImage->h;                // Hauteur du bouton

                // Dessiner le bouton sur la surface de l'écran
                SDL_BlitSurface(buttonImage, NULL, screenSurface, &buttonRect);

                // Libérer la surface de l'image du bouton
                SDL_FreeSurface(buttonImage);
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
                    else if (e.type == SDL_MOUSEBUTTONDOWN)
                    {
                        int x, y;
                        SDL_GetMouseState(&x, &y);
                        if (x >= buttonRect.x && x <= buttonRect.x + buttonRect.w && y >= buttonRect.y && y <= buttonRect.y + buttonRect.h)
                        {
                            // L'utilisateur a cliqué sur le bouton back
                            SDL_DestroyRenderer(renderer);
                            SDL_DestroyWindow(window);
                            SDL_Quit();
                            theMain();
                        }
                    }
                }
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}
