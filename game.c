#include "pacman.h"
#include <math.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* backgroundTexture = NULL;
SDL_Texture* playerTextures[4] = {NULL, NULL, NULL, NULL}; // Textures pour les 4 directions
SDL_Texture* ballTexture = NULL;
SDL_Texture* superballTexture = NULL;
SDL_Texture *ghost1Texture = NULL;
SDL_Texture *ghost2Texture = NULL;
SDL_Texture *ghost3Texture = NULL;
SDL_Texture *ghost4Texture = NULL;
SDL_Rect ghost1Rect, ghost2Rect, ghost3Rect, ghost4Rect;
SDL_Rect playerRect;

// Directions indices
enum { UP, DOWN, LEFT, RIGHT };

//initialisation de la map en 24 par 15

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

int isOverlap(SDL_Rect *rect1, SDL_Rect *rect2)
{
    if (rect1->x < rect2->x + rect2->w &&
        rect1->x + rect1->w > rect2->x &&
        rect1->y < rect2->y + rect2->h &&
        rect1->y + rect1->h > rect2->y)
    {
        return 1;
    }
    return 0;
}

void changeGhostDirection(int *dx, int *dy)
{
    int direction = rand() % 4;
    switch (direction)
    {
    case 0: // Haut
        *dx = 0;
        *dy = -SPEED;
        break;
    case 1: // Bas
        *dx = 0;
        *dy = SPEED;
        break;
    case 2: // Gauche
        *dx = -SPEED;
        *dy = 0;
        break;
    case 3: // Droite
        *dx = SPEED;
        *dy = 0;
        break;
    }
}

void addSuperball() {

    // Initialiser le générateur de nombres pseudo-aléatoires avec une graine différente
    srand(time(NULL));

    int count = 0;
    while (count < 4) {
        int x = rand() % MAP_WIDTH;
        int y = rand() % MAP_HEIGHT;
        if (map[y][x] == 0) {
            map[y][x] = 3; // Placer une superball
            count++;
        }
    }
}

void drawMap(SDL_Renderer* renderer) {
    // Load and render the background map image
    SDL_Surface* mapSurface = SDL_LoadBMP("./image/map.bmp");
    if (mapSurface == NULL) {
        printf("Failed to load map image! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    SDL_Texture* mapTexture = SDL_CreateTextureFromSurface(renderer, mapSurface);
    SDL_FreeSurface(mapSurface);
    SDL_RenderCopy(renderer, mapTexture, NULL, NULL);
    SDL_DestroyTexture(mapTexture);

    // Render the pokeballs on top of the background map
    SDL_Rect tileRect;
    tileRect.w = TILE_SIZE;
    tileRect.h = TILE_SIZE;

    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            tileRect.x = x * TILE_SIZE;
            tileRect.y = y * TILE_SIZE;
            if (map[y][x] != 1){
                if (map[y][x] == 0) {
                    // Render the texture for the pokeball
                    SDL_Rect textureRect;
                    textureRect.x = tileRect.x + (TILE_SIZE - 22) / 2;
                    textureRect.y = tileRect.y + (TILE_SIZE - 30) / 2;
                    textureRect.w = 25;
                    textureRect.h = 25;
                    SDL_RenderCopy(renderer, ballTexture, NULL, &textureRect);
                }
                else if (map[y][x] == 3) {
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

void initGame() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    // Create window
    window = SDL_CreateWindow("Pacman Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    // Load player textures for different directions
    SDL_Surface* playerSurfaces[4];
    playerSurfaces[UP] = SDL_LoadBMP("./sprites/draco_up.bmp");
    playerSurfaces[DOWN] = SDL_LoadBMP("./sprites/draco_down.bmp");
    playerSurfaces[LEFT] = SDL_LoadBMP("./sprites/draco_left.bmp");
    playerSurfaces[RIGHT] = SDL_LoadBMP("./sprites/draco_right.bmp");

    for (int i = 0; i < 4; ++i) {
        if (playerSurfaces[i] == NULL) {
            printf("Failed to load player image! SDL_Error: %s\n", SDL_GetError());
            return;
        }
        playerTextures[i] = SDL_CreateTextureFromSurface(renderer, playerSurfaces[i]);
        SDL_FreeSurface(playerSurfaces[i]);
    }

    // Load ball texture
    SDL_Surface* ballSurface = SDL_LoadBMP("./sprites/pokeball.bmp");
    if (ballSurface == NULL) {
        printf("Failed to load ball image! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    ballTexture = SDL_CreateTextureFromSurface(renderer, ballSurface);
    SDL_FreeSurface(ballSurface);

    // Load ball texture
    SDL_Surface* superballSurface = SDL_LoadBMP("./sprites/superball.bmp");
    if (superballSurface == NULL) {
        printf("Failed to load ball image! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    superballTexture = SDL_CreateTextureFromSurface(renderer, superballSurface);
    SDL_FreeSurface(superballSurface);

    // Dans votre fonction initGame(), ajoutez ces lignes pour charger les textures des fantômes
    SDL_Surface *ghost1Surface = SDL_LoadBMP("./sprites/ghost1.bmp");
    ghost1Texture = SDL_CreateTextureFromSurface(renderer, ghost1Surface);
    SDL_FreeSurface(ghost1Surface);

    SDL_Surface *ghost2Surface = SDL_LoadBMP("./sprites/ghost2.bmp");
    ghost2Texture = SDL_CreateTextureFromSurface(renderer, ghost2Surface);
    SDL_FreeSurface(ghost2Surface);

    SDL_Surface *ghost3Surface = SDL_LoadBMP("./sprites/ghost3.bmp");
    ghost3Texture = SDL_CreateTextureFromSurface(renderer, ghost3Surface);
    SDL_FreeSurface(ghost3Surface);

    SDL_Surface *ghost4Surface = SDL_LoadBMP("./sprites/ghost4.bmp");
    ghost4Texture = SDL_CreateTextureFromSurface(renderer, ghost4Surface);
    SDL_FreeSurface(ghost4Surface);

    // Set initial position of player
    playerRect.x = 650;
    playerRect.y = 350;
    playerRect.w = TILE_SIZE - 1;
    playerRect.h = TILE_SIZE - 1;

    // Définissez les positions initiales des fantômes
    ghost1Rect.x = 300;
    ghost1Rect.y = 300;
    ghost1Rect.w = TILE_SIZE - 1;
    ghost1Rect.h = TILE_SIZE - 1;
    ghost2Rect.x = 950;
    ghost2Rect.y = 200;
    ghost2Rect.w = TILE_SIZE - 1;
    ghost2Rect.h = TILE_SIZE - 1;
    ghost3Rect.x = 950;
    ghost3Rect.y = 500;
    ghost3Rect.w = TILE_SIZE - 1;
    ghost3Rect.h = TILE_SIZE - 1;
    ghost4Rect.x = 500;
    ghost4Rect.y = 500;
    ghost4Rect.w = TILE_SIZE - 1;
    ghost4Rect.h = TILE_SIZE - 1;

    // Variables to store the player's current tile
    int currentTileX = playerRect.x / TILE_SIZE;
    int currentTileY = playerRect.y / TILE_SIZE;

    // Définir la vitesse et la direction du personnage
    int speed = 10;
    int dx = 0, dy = 0;
    int last_dx = 0, last_dy = 0; // Ajouter ces lignes pour stocker la dernière direction valide
    int currentDirection = RIGHT; // Initial direction

    // Définir la vitesse et la direction des phantomes
    int ghost1_dx = 0, ghost1_dy = 0;
    int ghost2_dx = 0, ghost2_dy = 0;
    int ghost3_dx = 0, ghost3_dy = 0;
    int ghost4_dx = 0, ghost4_dy = 0;

    // Changez la direction des fantômes de manière aléatoire à chaque tour
    changeGhostDirection(&ghost1_dx, &ghost1_dy);
    changeGhostDirection(&ghost2_dx, &ghost2_dy);
    changeGhostDirection(&ghost3_dx, &ghost3_dy);
    changeGhostDirection(&ghost4_dx, &ghost4_dy);

    // Boucle principale
    SDL_Event event;
    int quit = 0;

    addSuperball();

    while (!quit) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
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

        // Calculer les nouvelles positions proposées pour tous les coins de la hitbox
        int newX1 = (playerRect.x + dx) / TILE_SIZE;
        int newY1 = (playerRect.y + dy) / TILE_SIZE;
        int newX2 = (playerRect.x + dx + playerRect.w) / TILE_SIZE;
        int newY2 = (playerRect.y + dy + playerRect.h) / TILE_SIZE;

        // Calculer les nouvelles positions proposées pour tous les coins de la hitbox du fantôme
        int newGhost1X1 = (ghost1Rect.x + ghost1_dx) / TILE_SIZE;
        int newGhost1Y1 = (ghost1Rect.y + ghost1_dy) / TILE_SIZE;
        int newGhost1X2 = (ghost1Rect.x + ghost1_dx + ghost1Rect.w) / TILE_SIZE;
        int newGhost1Y2 = (ghost1Rect.y + ghost1_dy + ghost1Rect.h) / TILE_SIZE;
        int newGhost2X1 = (ghost2Rect.x + ghost2_dx) / TILE_SIZE;
        int newGhost2Y1 = (ghost2Rect.y + ghost2_dy) / TILE_SIZE;
        int newGhost2X2 = (ghost2Rect.x + ghost2_dx + ghost2Rect.w) / TILE_SIZE;
        int newGhost2Y2 = (ghost2Rect.y + ghost2_dy + ghost2Rect.h) / TILE_SIZE;
        int newGhost3X1 = (ghost3Rect.x + ghost3_dx) / TILE_SIZE;
        int newGhost3Y1 = (ghost3Rect.y + ghost3_dy) / TILE_SIZE;
        int newGhost3X2 = (ghost3Rect.x + ghost3_dx + ghost3Rect.w) / TILE_SIZE;
        int newGhost3Y2 = (ghost3Rect.y + ghost3_dy + ghost3Rect.h) / TILE_SIZE;
        int newGhost4X1 = (ghost4Rect.x + ghost4_dx) / TILE_SIZE;
        int newGhost4Y1 = (ghost4Rect.y + ghost4_dy) / TILE_SIZE;
        int newGhost4X2 = (ghost4Rect.x + ghost4_dx + ghost4Rect.w) / TILE_SIZE;
        int newGhost4Y2 = (ghost4Rect.y + ghost4_dy + ghost4Rect.h) / TILE_SIZE;

        int collision = 0;
        // Vérifier si le mouvement proposé ferait entrer le fantôme 1 en collision avec un mur
        for (int y = newGhost1Y1; y <= newGhost1Y2 && !collision; ++y) {
            for (int x = newGhost1X1; x <= newGhost1X2 && !collision; ++x) {
                if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT && map[y][x] == 1) {
                    collision = 1;
                }
            }
        }
        if(!collision){
            // Si toutes les nouvelles positions sont des tuiles vides (0), alors déplacer le fantôme
            ghost1Rect.x += ghost1_dx;
            ghost1Rect.y += ghost1_dy;
        }else{
            // Si une collision se produit, changer la direction du fantôme
            changeGhostDirection(&ghost1_dx, &ghost1_dy);
        }

        // Vérifier si le mouvement proposé ferait entrer le fantôme 1 en collision avec un mur
        collision = 0;
        for (int y = newGhost2Y1; y <= newGhost2Y2 && !collision; ++y) {
            for (int x = newGhost2X1; x <= newGhost2X2 && !collision; ++x) {
                if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT && map[y][x] == 1) {
                    collision = 1;
                }
            }
        }
        if(!collision){
            // Si toutes les nouvelles positions sont des tuiles vides (0), alors déplacer le fantôme
            ghost2Rect.x += ghost2_dx;
            ghost2Rect.y += ghost2_dy;
        }else{
            // Si une collision se produit, changer la direction du fantôme
            changeGhostDirection(&ghost2_dx, &ghost2_dy);
        }

        // Vérifier si le mouvement proposé ferait entrer le fantôme 1 en collision avec un mur
        collision = 0;
        for (int y = newGhost3Y1; y <= newGhost3Y2 && !collision; ++y) {
            for (int x = newGhost3X1; x <= newGhost3X2 && !collision; ++x) {
                if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT && map[y][x] == 1) {
                    collision = 1;
                }
            }
        }
        if(!collision){
            // Si toutes les nouvelles positions sont des tuiles vides (0), alors déplacer le fantôme
            ghost3Rect.x += ghost3_dx;
            ghost3Rect.y += ghost3_dy;
        }else{
            // Si une collision se produit, changer la direction du fantôme
            changeGhostDirection(&ghost3_dx, &ghost3_dy);
        }

        // Vérifier si le mouvement proposé ferait entrer le fantôme 1 en collision avec un mur
        collision = 0;
        for (int y = newGhost4Y1; y <= newGhost4Y2 && !collision; ++y) {
            for (int x = newGhost4X1; x <= newGhost4X2 && !collision; ++x) {
                if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT && map[y][x] == 1) {
                    collision = 1;
                }
            }
        }
        if(!collision){
            // Si toutes les nouvelles positions sont des tuiles vides (0), alors déplacer le fantôme
            ghost4Rect.x += ghost4_dx;
            ghost4Rect.y += ghost4_dy;
        }else{
            // Si une collision se produit, changer la direction du fantôme
            changeGhostDirection(&ghost4_dx, &ghost4_dy);
        }

        // Vérifier si le mouvement proposé ferait entrer player en collision avec un mur
        collision = 0;
        for (int y = newY1; y <= newY2 && !collision; ++y) {
            for (int x = newX1; x <= newX2 && !collision; ++x) {
                if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT && map[y][x] == 1) {
                    collision = 1;
                }
            }
        }

        if (!collision) {
            // Si toutes les nouvelles positions sont des tuiles vides (0), déplacez le joueur
            playerRect.x += dx;
            playerRect.y += dy;
            last_dx = dx; // Mettre à jour la dernière direction valide
            last_dy = dy;

            // Dans votre boucle principale, vérifiez si le joueur a heurté un fantôme
            if (isOverlap(&playerRect, &ghost1Rect) || isOverlap(&playerRect, &ghost2Rect) || 
                isOverlap(&playerRect, &ghost3Rect) || isOverlap(&playerRect, &ghost4Rect)) {
                    quit = 1;
                }


            // Mettre à jour la carte pour montrer le chemin du joueur
            for (int y = newY1; y <= newY2; ++y) {
                for (int x = newX1; x <= newX2; ++x) {
                    if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT && map[y][x] != 2) {
                        if(map[y][x] == 3){
                            gameScore += 50; // Augmenter le score du jeu de 50 pour chaque superball collectée
                        }
                        else{
                            gameScore += 10; // Augmenter le score du jeu de 10 pour chaque pokeball collectée
                        }
                        map[y][x] = 2;
                    }
                }
            }
        } else {
            // Calculer les nouvelles positions proposées pour tous les coins de la hitbox en utilisant la dernière direction valide
            newX1 = (playerRect.x + last_dx) / TILE_SIZE;
            newY1 = (playerRect.y + last_dy) / TILE_SIZE;
            newX2 = (playerRect.x + last_dx + playerRect.w) / TILE_SIZE;
            newY2 = (playerRect.y + last_dy + playerRect.h) / TILE_SIZE;

            // Vérifier si le déplacement dans la dernière direction valide entraînerait une collision avec un mur
            collision = 0;
            for (int y = newY1; y <= newY2 && !collision; ++y) {
                for (int x = newX1; x <= newX2 && !collision; ++x) {
                    if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT && map[y][x] == 1) {
                        collision = 1;
                    }
                }
            }

            if (!collision) {
                // Si toutes les nouvelles positions sont des tuiles vides (0), déplacez le joueur
                playerRect.x += last_dx;
                playerRect.y += last_dy;

                // Mettre à jour la carte pour montrer le chemin du joueur
                for (int y = newY1; y <= newY2; ++y) {
                    for (int x = newX1; x <= newX2; ++x) {
                        if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT && map[y][x] != 2) {
                            if(map[y][x] == 3){
                                gameScore += 50; // Augmenter le score du jeu de 50 pour chaque superball collectée
                            }
                            else{
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

        // Ajoutez ces lignes pour rendre les textures des fantômes
        SDL_RenderCopy(renderer, ghost1Texture, NULL, &ghost1Rect);
        SDL_RenderCopy(renderer, ghost2Texture, NULL, &ghost2Rect);
        SDL_RenderCopy(renderer, ghost3Texture, NULL, &ghost3Rect);
        SDL_RenderCopy(renderer, ghost4Texture, NULL, &ghost4Rect);

        // Update the renderer
        SDL_RenderPresent(renderer);
    }

    // Clean up resources before exiting
    for (int i = 0; i < 4; ++i) {
        if (playerTextures[i] != NULL) {
            SDL_DestroyTexture(playerTextures[i]);
            playerTextures[i] = NULL;
        }
    }
    if (ballTexture != NULL) {
        SDL_DestroyTexture(ballTexture);
        ballTexture = NULL;
    }
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    if (window != NULL) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    SDL_Quit();
    theMain();
}
