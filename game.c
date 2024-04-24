#include "pacman.h"

const int DX[4] = {-1, 0, 1, 0};
const int DY[4] = {0, -1, 0, 1};

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *backgroundTexture = NULL;
SDL_Texture *playerTexture = NULL;
SDL_Texture *ghost1Texture = NULL;
SDL_Texture *ghost2Texture = NULL;
SDL_Texture *ghost3Texture = NULL;
SDL_Texture *ghost4Texture = NULL;

SDL_Rect ghost1Rect;
SDL_Rect ghost2Rect;
SDL_Rect ghost3Rect;
SDL_Rect ghost4Rect;
SDL_Rect playerRect;

// Prototypes de fonction
void gameOver(SDL_Renderer *renderer);
void drawLetter(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y);
// initialisation de la map en 24 par 15

int map[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 2, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 2, 1, 2, 0, 0, 2, 0, 2, 0, 0, 0, 2, 0, 2, 1},
    {1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 2, 1, 0, 1, 1, 1, 0, 1, 0, 1},
    {1, 2, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 2, 2, 0, 2, 0, 0, 0, 0, 2, 2, 0, 2, 1},
    {1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1},
    {1, 2, 0, 2, 1, 1, 2, 0, 0, 2, 1, 1, 0, 1, 1, 0, 1, 2, 0, 2, 0, 0, 2, 0, 0, 2, 1},
    {1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 2, 2, 2, 0, 2, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1},
    {1, 1, 1, 2, 0, 0, 2, 0, 0, 2, 1, 0, 1, 0, 1, 2, 0, 2, 0, 0, 2, 1, 0, 1, 1, 1, 1},
    {0, 2, 0, 2, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 2, 0, 2, 0, 0, 2, 0},
    {1, 1, 1, 0, 1, 2, 2, 0, 0, 2, 1, 2, 0, 2, 0, 2, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1},
    {1, 2, 0, 2, 1, 0, 1, 1, 1, 2, 0, 2, 1, 1, 1, 0, 1, 2, 0, 2, 2, 0, 2, 0, 0, 2, 1},
    {1, 0, 1, 2, 0, 2, 1, 0, 0, 2, 1, 1, 1, 1, 2, 2, 0, 2, 1, 0, 1, 1, 0, 1, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 2, 0, 0, 2, 0, 2, 1, 1, 0, 1, 0, 1, 0, 2, 1, 1, 0, 1},
    {1, 2, 0, 2, 1, 0, 1, 0, 0, 2, 1, 1, 0, 1, 1, 1, 1, 2, 0, 2, 1, 1, 2, 0, 0, 2, 1},
    {1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 2, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 2, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},

    // Voir combien de pixel fait une case, c'est ca qui cause le décalage
};

int isCollision(int x, int y)
{
    int tileX = x / TILE_SIZE;
    int tileY = y / TILE_SIZE;
    return map[tileY][tileX] == 1;
}

void drawMap(SDL_Renderer *renderer)
{
    SDL_Rect tileRect;
    tileRect.w = TILE_SIZE;
    tileRect.h = TILE_SIZE;

    for (int y = 0; y < MAP_HEIGHT; ++y)
    {
        for (int x = 0; x < MAP_WIDTH; ++x)
        {
            tileRect.x = x * TILE_SIZE;
            tileRect.y = y * TILE_SIZE;

            if (map[y][x] == 0)
            {
                // Draw background image
                SDL_RenderCopy(renderer, backgroundTexture, NULL, &tileRect);
            }
        }
    }
}

// Définir la direction et la distance pour les ghosts

int ghost1Direction = 1; // 1 pour la droite, -1 pour la gauche
int ghost1Distance = 0;  // Distance parcourue depuis le dernier changement de direction

int ghost2Direction = 1; // 1 pour la droite, -1 pour la gauche
int ghost2Distance = 0;  // Distance parcourue depuis le dernier changement de direction

int ghost3Direction = 1; // 1 pour le bas, -1 pour le haut
int ghost3Distance = 0;  // Distance parcourue depuis le dernier changement de direction

int ghost4Direction = 1; // 1 pour le bas, -1 pour le haut
int ghost4Distance = 0;  // Distance parcourue depuis le dernier changement de direction

void initGame()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    // Create window
    window = SDL_CreateWindow("Pacman Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    SDL_Surface *backgroundSurface = SDL_LoadBMP("./sprites/Maps_1.bmp");
    if (backgroundSurface == NULL)
    {
        printf("Failed to load background image! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    // load player texture
    SDL_Surface *playerSurface = SDL_LoadBMP("./sprites/draco.bmp");
    if (playerSurface == NULL)
    {
        printf("Failed to load player image! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
    SDL_FreeSurface(playerSurface);

    // Load ghost1 texture
    SDL_Surface *ghost1Surface = SDL_LoadBMP("./sprites/ghost1.bmp");
    if (ghost1Surface == NULL)
    {
        printf("Failed to load ghost1 image! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    ghost1Texture = SDL_CreateTextureFromSurface(renderer, ghost1Surface);
    SDL_FreeSurface(ghost1Surface);

    // Load ghost2 texture
    SDL_Surface *ghost2Surface = SDL_LoadBMP("./sprites/ghost2.bmp");
    if (ghost2Surface == NULL)
    {
        printf("Failed to load ghost2 image! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    ghost2Texture = SDL_CreateTextureFromSurface(renderer, ghost2Surface);
    SDL_FreeSurface(ghost2Surface);

    // Load ghost3 texture
    SDL_Surface *ghost3Surface = SDL_LoadBMP("./sprites/ghost3.bmp");
    if (ghost3Surface == NULL)
    {
        printf("Failed to load ghost3 image! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    ghost3Texture = SDL_CreateTextureFromSurface(renderer, ghost3Surface);
    SDL_FreeSurface(ghost3Surface);

    // Load ghost4 texture
    SDL_Surface *ghost4Surface = SDL_LoadBMP("./sprites/ghost4.bmp");
    if (ghost4Surface == NULL)
    {
        printf("Failed to load ghost4 image! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    ghost4Texture = SDL_CreateTextureFromSurface(renderer, ghost4Surface);
    SDL_FreeSurface(ghost4Surface);

    // Set initial position of player
    playerRect.x = 650;
    playerRect.y = 350;
    playerRect.w = playerSurface->w;
    playerRect.h = playerSurface->h;
    playerRect.w = TILE_SIZE - 1;
    playerRect.h = TILE_SIZE - 1;

    // Set initial position of ghost1
    ghost1Rect.x = 450;
    ghost1Rect.y = 500;
    ghost1Rect.w = ghost1Surface->w;
    ghost1Rect.h = ghost1Surface->h;
    ghost1Rect.w = TILE_SIZE - 1;
    ghost1Rect.h = TILE_SIZE - 1;

    // Set initial position of ghost2
    ghost2Rect.x = 100;
    ghost2Rect.y = 50;
    ghost2Rect.w = ghost2Surface->w;
    ghost2Rect.h = ghost2Surface->h;
    ghost2Rect.w = TILE_SIZE - 1;
    ghost2Rect.h = TILE_SIZE - 1;

    // Set initial position of ghost3
    ghost3Rect.x = 850;
    ghost3Rect.y = 450;
    ghost3Rect.w = ghost3Surface->w;
    ghost3Rect.h = ghost3Surface->h;
    ghost3Rect.w = TILE_SIZE - 1;
    ghost3Rect.h = TILE_SIZE - 1;

    // Set initial position of ghost4
    ghost4Rect.x = 850;
    ghost4Rect.y = 500;
    ghost4Rect.w = ghost4Surface->w;
    ghost4Rect.h = ghost4Surface->h;
    ghost4Rect.w = TILE_SIZE - 1;
    ghost4Rect.h = TILE_SIZE - 1;

    // Définir la vitesse et la direction du personnage
    int speed = 10;
    int dx = 0, dy = 0;
    int last_dx = 0, last_dy = 0; // Ajouter ces lignes pour stocker la dernière direction valide

    // Boucle principale
    SDL_Event event;
    int quit = 0;
    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = 1;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
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
        int newX1 = (playerRect.x + dx) / TILE_SIZE;
        int newY1 = (playerRect.y + dy) / TILE_SIZE;
        int newX2 = (playerRect.x + dx + playerRect.w) / TILE_SIZE;
        int newY2 = (playerRect.y + dy + playerRect.h) / TILE_SIZE;

        // Vérifier si le mouvement proposé ferait entrer player en collision avec un mur
        if (map[newY1][newX1] == 0 && map[newY1][newX2] == 0 && map[newY2][newX1] == 0 && map[newY2][newX2] == 0)
        {
            // Si toutes les nouvelles positions sont des cases vides (0), alors déplacer player
            playerRect.x += dx;
            playerRect.y += dy;
            last_dx = dx; // Mettre à jour la dernière direction valide
            last_dy = dy;
        }
        else
        {
            // Calculer les nouvelles positions proposées pour tous les coins de la hitbox en utilisant la dernière direction valide
            newX1 = (playerRect.x + last_dx) / TILE_SIZE;
            newY1 = (playerRect.y + last_dy) / TILE_SIZE;
            newX2 = (playerRect.x + last_dx + playerRect.w) / TILE_SIZE;
            newY2 = (playerRect.y + last_dy + playerRect.h) / TILE_SIZE;

            // Vérifier si le mouvement dans la dernière direction valide ferait entrer player en collision avec un mur
            if (map[newY1][newX1] == 0 && map[newY1][newX2] == 0 && map[newY2][newX1] == 0 && map[newY2][newX2] == 0)
            {
                // Si toutes les nouvelles positions sont des cases vides (0), alors déplacer player
                playerRect.x += last_dx;
                playerRect.y += last_dy;
            }
            // Si une collision se produit, ne pas déplacer player
        }

        // Mettre à jour la position du fantôme
        const int GHOST_SPEED = 10;            // Vitesse du fantôme en pixels par frame
        const int GHOST_TRAVEL_DISTANCE = 100; // Distance à parcourir avant de changer de direction
        ghost1Rect.x += GHOST_SPEED * ghost1Direction;
        ghost1Distance += GHOST_SPEED;

        // Vérifier si le fantôme a parcouru la distance souhaitée
        if (ghost1Distance >= GHOST_TRAVEL_DISTANCE)
        {
            // Changer de direction et réinitialiser la distance parcourue
            ghost1Direction *= -1;
            ghost1Distance = 0;
        }

        // Mettre à jour la position de ghost2 (gauche-droite)
        ghost2Rect.x += GHOST_SPEED * ghost2Direction;
        ghost2Distance += GHOST_SPEED;

        // Vérifier si ghost2 a parcouru la distance souhaitée
        if (ghost2Distance >= 300) // Remplacer par la distance que vous voulez
        {
            // Changer de direction et réinitialiser la distance parcourue
            ghost2Direction *= -1;
            ghost2Distance = 0;
        }

        // Mettre à jour la position de ghost3 (haut-bas)
        ghost3Rect.y += GHOST_SPEED * ghost3Direction;
        ghost3Distance += GHOST_SPEED;

        // Vérifier si ghost3 a parcouru la distance souhaitée
        if (ghost3Distance >= 300) // Remplacer par la distance que vous voulez
        {
            // Changer de direction et réinitialiser la distance parcourue
            ghost3Direction *= -1;
            ghost3Distance = 0;
        }

        // Mettre à jour la position de ghost4
        if (ghost4Direction < 2)
        {
            ghost4Rect.x += GHOST_SPEED * DX[ghost4Direction];
        }
        else
        {
            ghost4Rect.y += GHOST_SPEED * DY[ghost4Direction];
        }

        // Vérifier si ghost4 est à un croisement
        int tileX = ghost4Rect.x / TILE_SIZE;
        int tileY = ghost4Rect.y / TILE_SIZE;
        if (map[tileY][tileX] == 2)
        {
            // Choisir une direction valide au hasard
            int newDirection;
            do
            {
                newDirection = rand() % 4; // Choisir une direction au hasard
                int newX = ghost4Rect.x + DX[newDirection] * GHOST_SPEED;
                int newY = ghost4Rect.y + DY[newDirection] * GHOST_SPEED;
                tileX = newX / TILE_SIZE;
                tileY = newY / TILE_SIZE;
            } while (map[tileY][tileX] == 1); // Continuer tant que la direction choisie n'est pas valide (0 ou 2)

            // Mettre à jour la direction de ghost4
            ghost4Direction = newDirection;
        }
        // Vérifier la collision avec chaque fantôme
        if (SDL_HasIntersection(&playerRect, &ghost1Rect) ||
            SDL_HasIntersection(&playerRect, &ghost2Rect) ||
            SDL_HasIntersection(&playerRect, &ghost3Rect) ||
            SDL_HasIntersection(&playerRect, &ghost4Rect))
        {
            // Si une collision est détectée, afficher "Game Over"
            gameOver(renderer);
        }

        // Effacer le rendu
        SDL_RenderClear(renderer);

        // Rendre la texture de fond
        // SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        drawMap(renderer);

        // Rendre les textures
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);
        SDL_RenderCopy(renderer, ghost1Texture, NULL, &ghost1Rect);
        SDL_RenderCopy(renderer, ghost2Texture, NULL, &ghost2Rect);
        SDL_RenderCopy(renderer, ghost3Texture, NULL, &ghost3Rect);
        SDL_RenderCopy(renderer, ghost4Texture, NULL, &ghost4Rect);

        // Mettre à jour le rendu
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(ghost1Texture);
    SDL_DestroyTexture(ghost2Texture);
    SDL_DestroyTexture(ghost3Texture);
    SDL_DestroyTexture(ghost4Texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void gameOver(SDL_Renderer *renderer)
{
    // Charger chaque lettre
    SDL_Texture *letters[8];
    const char *letterFiles[8] = {"./lettres/G.bmp", "./lettres/A.bmp", "./lettres/M.bmp", "./lettres/E.bmp",
                                  "./lettres/O.bmp", "./lettres/V.bmp", "./lettres/E.bmp", "./lettres/R.bmp"};
    for (int i = 0; i < 8; ++i)
    {
        SDL_Surface *letterSurface = SDL_LoadBMP(letterFiles[i]);
        if (letterSurface == NULL)
        {
            printf("Failed to load letter image! SDL_Error: %s\n", SDL_GetError());
            return;
        }
        letters[i] = SDL_CreateTextureFromSurface(renderer, letterSurface);
        SDL_FreeSurface(letterSurface);
    }

    // Calculer la position initiale pour centrer "Game Over"
    int startX = WINDOW_WIDTH / 2 - (7 * TILE_SIZE) / 2;
    int startY = WINDOW_HEIGHT / 2 - TILE_SIZE / 2;

    // Afficher chaque lettre une par une
    for (int i = 0; i < 8; ++i)
    {
        drawLetter(renderer, letters[i], startX + i * TILE_SIZE, startY);
        SDL_RenderPresent(renderer);
        SDL_Delay(500); // Attendre 500 ms entre chaque lettre
    }

    // Attendre 2 secondes avant de continuer
    SDL_Delay(2000);

    // Libérer les textures
    for (int i = 0; i < 8; ++i)
    {
        SDL_DestroyTexture(letters[i]);
    }
}

void drawLetter(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = TILE_SIZE;
    rect.h = TILE_SIZE;
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}