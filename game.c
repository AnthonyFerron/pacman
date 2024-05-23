#include "pacman.h"
#include "saveScores.h"
#include <math.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *backgroundTexture = NULL;
SDL_Texture *playerTextures[4] = {NULL, NULL, NULL, NULL}; // Textures pour les 4 directions
SDL_Texture *ballTexture = NULL;
SDL_Texture *superballTexture = NULL;
SDL_Rect playerRect;

// Directions indices
enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// initialisation de la map en 24 par 15

int map[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

int checkRemainingPokeballs()
{
    for (int y = 0; y < MAP_HEIGHT; ++y)
    {
        for (int x = 0; x < MAP_WIDTH; ++x)
        {
            if (map[y][x] == 0 || map[y][x] == 3)
            {             // 0 pour pokeball, 3 pour superball
                return 1; // Il reste encore des pokeballs ou superballs
            }
        }
    }
    return 0; // Il n'y a plus de pokeballs ou superballs
}

int isCollision(int x, int y)
{
    int tileX = x / TILE_SIZE;
    int tileY = y / TILE_SIZE;
    return map[tileY][tileX] == 1;
}

void addSuperball()
{

    // Initialiser le générateur de nombres pseudo-aléatoires avec une graine différente
    srand(time(NULL));

    int count = 0;
    while (count < 4)
    {
        int x = rand() % MAP_WIDTH;
        int y = rand() % MAP_HEIGHT;
        if (map[y][x] == 0)
        {
            map[y][x] = 3; // Placer une superball
            count++;
        }
    }
}

void drawMap(SDL_Renderer *renderer)
{
    // Load and render the background map image
    SDL_Surface *mapSurface = SDL_LoadBMP("./image/map.bmp");
    if (mapSurface == NULL)
    {
        printf("Failed to load map image! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    SDL_Texture *mapTexture = SDL_CreateTextureFromSurface(renderer, mapSurface);
    SDL_FreeSurface(mapSurface);
    SDL_RenderCopy(renderer, mapTexture, NULL, NULL);
    SDL_DestroyTexture(mapTexture);

    // Render the pokeballs on top of the background map
    SDL_Rect tileRect;
    tileRect.w = TILE_SIZE;
    tileRect.h = TILE_SIZE;

    for (int y = 0; y < MAP_HEIGHT; ++y)
    {
        for (int x = 0; x < MAP_WIDTH; ++x)
        {
            tileRect.x = x * TILE_SIZE;
            tileRect.y = y * TILE_SIZE;
            if (map[y][x] != 1)
            {
                if (map[y][x] == 0)
                {
                    // Render the texture for the pokeball
                    SDL_Rect textureRect;
                    textureRect.x = tileRect.x + (TILE_SIZE - 22) / 2;
                    textureRect.y = tileRect.y + (TILE_SIZE - 30) / 2;
                    textureRect.w = 25;
                    textureRect.h = 25;
                    SDL_RenderCopy(renderer, ballTexture, NULL, &textureRect);
                }
                else if (map[y][x] == 3)
                {
                    // Render the texture for the superball
                    SDL_Rect textureRect;
                    textureRect.x = tileRect.x + (TILE_SIZE - 28) / 2;
                    textureRect.y = tileRect.y + (TILE_SIZE - 35) / 2;
                    textureRect.w = 30;
                    textureRect.h = 30;
                    SDL_RenderCopy(renderer, superballTexture, NULL, &textureRect);
                }
            }
        }
    }
}

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

    // Load player textures for different directions
    SDL_Surface *playerSurfaces[4];
    playerSurfaces[UP] = SDL_LoadBMP("./sprites/draco_up.bmp");
    playerSurfaces[DOWN] = SDL_LoadBMP("./sprites/draco_down.bmp");
    playerSurfaces[LEFT] = SDL_LoadBMP("./sprites/draco_left.bmp");
    playerSurfaces[RIGHT] = SDL_LoadBMP("./sprites/draco_right.bmp");

    for (int i = 0; i < 4; ++i)
    {
        if (playerSurfaces[i] == NULL)
        {
            printf("Failed to load player image! SDL_Error: %s\n", SDL_GetError());
            return;
        }
        playerTextures[i] = SDL_CreateTextureFromSurface(renderer, playerSurfaces[i]);
        SDL_FreeSurface(playerSurfaces[i]);
    }

    // Load ball texture
    SDL_Surface *ballSurface = SDL_LoadBMP("./sprites/pokeball.bmp");
    if (ballSurface == NULL)
    {
        printf("Failed to load ball image! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    ballTexture = SDL_CreateTextureFromSurface(renderer, ballSurface);
    SDL_FreeSurface(ballSurface);

    // Load ball texture
    SDL_Surface *superballSurface = SDL_LoadBMP("./sprites/superball.bmp");
    if (superballSurface == NULL)
    {
        printf("Failed to load ball image! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    superballTexture = SDL_CreateTextureFromSurface(renderer, superballSurface);
    SDL_FreeSurface(superballSurface);

    // Set initial position of player
    playerRect.x = 650;
    playerRect.y = 350;
    playerRect.w = TILE_SIZE - 1;
    playerRect.h = TILE_SIZE - 1;

    // Variables to store the player's current tile
    int currentTileX = playerRect.x / TILE_SIZE;
    int currentTileY = playerRect.y / TILE_SIZE;

    // Définir la vitesse et la direction du personnage
    int speed = 50;
    int dx = 0, dy = 0;
    int last_dx = 0, last_dy = 0; // Ajouter ces lignes pour stocker la dernière direction valide
    int currentDirection = RIGHT; // Initial direction

    // Boucle principale
    SDL_Event event;
    int quit = 0;

    addSuperball();

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
                case SDLK_ESCAPE:
                    quit = 1;
                    break;
                case SDLK_UP:
                    dx = 0;
                    dy = -speed;
                    currentDirection = UP;
                    break;
                case SDLK_DOWN:
                    dx = 0;
                    dy = speed;
                    currentDirection = DOWN;
                    break;
                case SDLK_LEFT:
                    dx = -speed;
                    dy = 0;
                    currentDirection = LEFT;
                    break;
                case SDLK_RIGHT:
                    dx = speed;
                    dy = 0;
                    currentDirection = RIGHT;
                    break;
                }
            }
        }

        if (!checkRemainingPokeballs())
        {
            saveScores(gameScore); // Appeler la fonction pour sauvegarder les scores
            gameScore = 0;
        }

        // Calculer les nouvelles positions proposées pour tous les coins de la hitbox
        int newX1 = (playerRect.x + dx) / TILE_SIZE;
        int newY1 = (playerRect.y + dy) / TILE_SIZE;
        int newX2 = (playerRect.x + dx + playerRect.w) / TILE_SIZE;
        int newY2 = (playerRect.y + dy + playerRect.h) / TILE_SIZE;

        // Vérifier si le mouvement proposé ferait entrer player en collision avec un mur
        int collision = 0;
        for (int y = newY1; y <= newY2 && !collision; ++y)
        {
            for (int x = newX1; x <= newX2 && !collision; ++x)
            {
                if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT && map[y][x] == 1)
                {
                    collision = 1;
                }
            }
        }

        if (!collision)
        {
            // Si toutes les nouvelles positions sont des tuiles vides (0), déplacez le joueur
            playerRect.x += dx;
            playerRect.y += dy;
            last_dx = dx; // Mettre à jour la dernière direction valide
            last_dy = dy;

            // Mettre à jour la carte pour montrer le chemin du joueur
            for (int y = newY1; y <= newY2; ++y)
            {
                for (int x = newX1; x <= newX2; ++x)
                {
                    if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT && map[y][x] != 2)
                    {
                        if (map[y][x] == 3)
                        {
                            gameScore += 50; // Augmenter le score du jeu de 50 pour chaque superball collectée
                        }
                        else
                        {
                            gameScore += 10; // Augmenter le score du jeu de 10 pour chaque pokeball collectée
                        }
                        map[y][x] = 2;
                    }
                }
            }
        }
        else
        {
            // Calculer les nouvelles positions proposées pour tous les coins de la hitbox en utilisant la dernière direction valide
            newX1 = (playerRect.x + last_dx) / TILE_SIZE;
            newY1 = (playerRect.y + last_dy) / TILE_SIZE;
            newX2 = (playerRect.x + last_dx + playerRect.w) / TILE_SIZE;
            newY2 = (playerRect.y + last_dy + playerRect.h) / TILE_SIZE;

            // Vérifier si le déplacement dans la dernière direction valide entraînerait une collision avec un mur
            collision = 0;
            for (int y = newY1; y <= newY2 && !collision; ++y)
            {
                for (int x = newX1; x <= newX2 && !collision; ++x)
                {
                    if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT && map[y][x] == 1)
                    {
                        collision = 1;
                    }
                }
            }

            if (!collision)
            {
                // Si toutes les nouvelles positions sont des tuiles vides (0), déplacez le joueur
                playerRect.x += last_dx;
                playerRect.y += last_dy;

                // Mettre à jour la carte pour montrer le chemin du joueur
                for (int y = newY1; y <= newY2; ++y)
                {
                    for (int x = newX1; x <= newX2; ++x)
                    {
                        if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT && map[y][x] != 2)
                        {
                            if (map[y][x] == 3)
                            {
                                gameScore += 50; // Augmenter le score du jeu de 50 pour chaque superball collectée
                            }
                            else
                            {
                                gameScore += 10; // Augmenter le score du jeu de 10 pour chaque pokeball collectée
                            }
                            map[y][x] = 2;
                        }
                    }
                }
            }
        }

        // Clear the renderer
        SDL_RenderClear(renderer);

        // Render the background texture
        drawMap(renderer);

        // Render the player texture based on the current direction
        SDL_RenderCopy(renderer, playerTextures[currentDirection], NULL, &playerRect);

        // Update the renderer
        SDL_RenderPresent(renderer);
    }

    // Clean up resources before exiting
    for (int i = 0; i < 4; ++i)
    {
        if (playerTextures[i] != NULL)
        {
            SDL_DestroyTexture(playerTextures[i]);
            playerTextures[i] = NULL;
        }
    }
    if (ballTexture != NULL)
    {
        SDL_DestroyTexture(ballTexture);
        ballTexture = NULL;
    }
    if (renderer != NULL)
    {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    if (window != NULL)
    {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    SDL_Quit();
    theMain();
}
