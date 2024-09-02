#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define HEIGHT 500
#define WIDTH 500

static SDL_Window *window; /* Main window for the program */
static SDL_Surface *surface; /* Surface for modifying */
static SDL_Texture *texture; /* Texture we use for rendering */

static SDL_bool done = SDL_FALSE;
static long count = 0;
static unsigned short color[4];

//int event_handler(void *user, SDL_Event *event)
//{
//    if (event->type == SDL_MOUSEBUTTONDOWN) {
//        done = SDL_TRUE;
//        return 0;
//    }
//    return 0;
//}

void loop(void *arg)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) { 
            done = SDL_TRUE;
            return;
        }
        else if (event.type == SDL_KEYUP) {
            if (event.key.keysym.sym == SDL_GetKeyFromName("q")) {
                done = SDL_TRUE;
                return;
            }
        }

    }
    SDL_Renderer *renderer = (SDL_Renderer *)arg;
    //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); /* set color to white */
    /* Generate random coordinate */
    int x = rand() % WIDTH;
    int y = rand() % HEIGHT;
    color[0] = rand() % 256;
    //color[0] = 75;
    color[1] = rand() % 256;
    color[2] = rand() % 256;
    color[3] = 0;
    

    int location = (y * HEIGHT * 4) + (x * 4); 
    memcpy(surface->pixels + location, color, 4);
    if (location - 4 >= 0) 
        memcpy(surface->pixels + location - 4, color, 4);
   
    if (location + 4 < WIDTH * HEIGHT * 4)  
        memcpy(surface->pixels + location + 4, color, 4);

    if (location - (HEIGHT * 4) >= 0)
        memcpy(surface->pixels + location - (HEIGHT * 4), color, 4);

    if (location + (HEIGHT * 4) < WIDTH * HEIGHT * 4)
        memcpy(surface->pixels + location + (HEIGHT * 4), color, 4);
    //memset(surface->pixels + location, 255, 4);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    //SDL_UpdateWindowSurface(window);
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
} 

int main(int argc, char *argv[])
{
    srand(time(NULL));
    SDL_Renderer *renderer; /* Rendering context */

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0) { /* Initialize SDL */
        exit(1);
    }

    window = SDL_CreateWindow("Test Window", 0, 0, WIDTH, HEIGHT, 0); /* Create a new window */
    if (!window) {
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, 0); /* Create a new rendering context */
    if (!renderer) {
        SDL_DestroyWindow(window);
        exit(1);
    }

    //SDL_AddEventWatch(event_handler, NULL);

    SDL_Rect rect = {
        .x = 0,
        .y = 0,
        .h = 300,
        .w = 300,
    };
 
    //surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
    surface = SDL_GetWindowSurface(window);
    //for (int x = 0; x < 300; x++) {
    //    for (int y = 0; y < 300; y++) {
    //        memset(surface->pixels + 
    memset(surface->pixels, 0, WIDTH * HEIGHT * 4);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    //SDL_UpdateWindowSurface(window);
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);

    //SDL_RenderDrawRect(renderer, &rect);
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); /* set color to black */
    //SDL_RenderFillRect(renderer, &rect); /* Fill screen with black */
    //SDL_RenderPresent(renderer); /* Render the queued actions */

    while (!done) {
        loop(renderer);
        SDL_Delay(1);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
