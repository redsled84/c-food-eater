/**
 * Name: food-eater.c
 * Description: Move a rectangle and eat food dood
 * Author: Lucas B. Black
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
#define FPS           60
#define MOVE_SPEED    350

int main(void)
{
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("Interactive Red Square",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        WINDOW_WIDTH, WINDOW_HEIGHT,
                                        SDL_WINDOW_OPENGL);

    if (!win) {
        printf("Unable to create window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_SetWindowBordered(win, SDL_FALSE);

    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
    if (!rend) {
        printf("Unable to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    // load image from memory
    SDL_Surface* surface = IMG_Load("FF0000.png");
    if (!surface) {
        printf("Unable to create surface\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    // load image data into graphic's memory
    SDL_Texture* player_tex = SDL_CreateTextureFromSurface(rend, surface);
    if(!player_tex) {
        printf("Unable to create texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    surface = IMG_Load("FFFF00.png");
    if (!surface) {
        printf("Unable to create surface\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    // load image data into graphic's memory
    SDL_Texture* food_tex = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);
    if(!food_tex) {
        printf("Unable to create texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    printf("Successfully initialized SDL\n");

    SDL_SetWindowBordered(win, SDL_TRUE);

    int score = 0;

    SDL_Rect player;
    // store the dimensions of the image into the rect
    SDL_QueryTexture(player_tex, NULL, NULL, &player.w, &player.h);
    player.w /= 4;
    player.h /= 4;

    SDL_Rect food;
    SDL_QueryTexture(food_tex, NULL, NULL, &food.w, &food.h);
    food.w /= 8;
    food.h /= 8;
    food.x = rand() % (WINDOW_WIDTH - food.w);
    food.y = rand() % (WINDOW_HEIGHT - food.h);

    float y_pos = (WINDOW_HEIGHT - player.h) / 2;
    float x_pos = (WINDOW_WIDTH - player.w) / 2;

    float x_vel = 0;
    float y_vel = 0;

    // 0 = W, 1 = A, 2 = S, 3 = D
    int keys[4] = {0};

    int is_running = 1;
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);

    while (is_running) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = 0;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    keys[0] = 1;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    keys[1] = 1;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    keys[2] = 1;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    keys[3] = 1;
                    break;
                case SDL_SCANCODE_ESCAPE:
                    is_running = 0;
                    break;
                default:
                    break;
                }
            } else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    keys[0] = 0;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    keys[1] = 0;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    keys[2] = 0;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    keys[3] = 0;
                    break;
                default:
                    break;
                }
            }
        }

        SDL_RenderClear(rend);

        player.y = (int) y_pos;
        player.x = (int) x_pos;

        // draw image
        SDL_RenderCopy(rend, player_tex, NULL, &player);
        SDL_RenderCopy(rend, food_tex, NULL, &food);
        SDL_RenderPresent(rend);

        // player update
        if (keys[0])
            y_vel = (float) MOVE_SPEED / FPS;
        else if (keys[2])
            y_vel = (float) -MOVE_SPEED / FPS;
        else
            y_vel = 0;

        if (keys[1])
            x_vel = (float) MOVE_SPEED / FPS;
        else if (keys[3])
            x_vel = (float) -MOVE_SPEED / FPS;
        else
            x_vel = 0;

        x_pos -= x_vel;
        y_pos -= y_vel;

        if (x_pos < 0) {
            x_pos = 0;
            x_vel = 0;
        } else if (x_pos + player.w > WINDOW_WIDTH) {
            x_pos = WINDOW_WIDTH - player.w;
        }
        if (y_pos < 0) {
            y_pos = 0;
            y_vel = 0;
        } else if (y_pos + player.h > WINDOW_HEIGHT) {
            y_pos = WINDOW_HEIGHT - player.h;
        }

        // food update
        if (player.x + player.w > food.x &&
        player.x < food.x + food.w       &&
        player.y + player.h > food.y     &&
        player.y < food.y + food.h) {
            score++;
            printf("Score: %4d\n", score);

            if (score > 9) {
                printf("You Won!\n");
                is_running = 0;
            }
            
            food.x = rand() % (WINDOW_WIDTH - food.w);
            food.y = rand() % (WINDOW_HEIGHT - food.h);
        }


        SDL_PumpEvents();
        SDL_Delay(1000 / FPS);

        if (player.y < -player.h)
            is_running = 0;
    }

    SDL_DestroyTexture(player_tex);
    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(rend);
    SDL_Quit();
}
