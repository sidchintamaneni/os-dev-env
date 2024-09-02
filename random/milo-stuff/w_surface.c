#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 500
#define HEIGHT 500

static SDL_Window *window;
static SDL_Surface *surface;
static SDL_Renderer *renderer;

static SDL_bool done = SDL_FALSE;

static int r = 0;
static int g = 0;
static int b = 0;

static inline void setPixel(SDL_Surface *sur, int x, int y, Uint32 value)
{
    Uint32 *pix = (Uint32 *)sur->pixels;
    pix[y * HEIGHT + x] = value;
}

static Uint32 newColor()
{
    Uint32 color = 0;
    if (r) 
        color = color | r << 16;
    else 
        color = color | rand() % 256 << 16;

    if (g) 
        color = color | g << 8;
    else 
        color = color | rand() % 256 << 8;

    if (b) 
        color = color | b;
    else 
        color = color | rand() % 256;
    return color;
}


int main(int argc, char *argv[])
{
    if (argc == 4) {
        r = atoi(argv[1]);
        g = atoi(argv[2]);
        b = atoi(argv[3]);
    }

    srand(time(NULL));
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
        exit(-1);

    window = SDL_CreateWindow("surface", 0, 0, WIDTH, HEIGHT, 0);
    surface = SDL_GetWindowSurface(window);
    if (SDL_MUSTLOCK(surface)) {
        SDL_LockSurface(surface);
    }

    Uint32 color = 0x00000000;
    color = rand() % 256;
   
    SDL_Event e;
  
    int segments = HEIGHT;
    int row_width = WIDTH;
    int row_height = HEIGHT;

    
    int step = 2;
    int sign = -1;
    

    while (!done) {
        for (int rows = 0; rows < HEIGHT / segments; rows++) { 
            for (int i = (WIDTH - row_width) / 2; i < WIDTH - ((WIDTH - row_width) / 2); i++) {
                for (int j = (HEIGHT - row_height) / 2; j < HEIGHT - ((HEIGHT - row_height) / 2); j++) {
                    setPixel(surface, i, j, color);
                }
                //color = (rand() % 256 << 16) | (rand() % 256 << 8) | (rand() % 256); 
                //color = (rand() % 256 << 16) | 0x00007300 | (rand() % 256); 
                color = newColor();
            }
        }
        SDL_UpdateWindowSurface(window);
        SDL_Delay(10);
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                done = SDL_TRUE;
            }
        }
        memset(surface->pixels, 0, HEIGHT * WIDTH * 4);
        SDL_UpdateWindowSurface(window);
        
        row_width += step * sign;
        row_height += step * sign;
        if (row_height <= 0) {
            sign = -1 * sign;
        }
        else if (row_height > HEIGHT) {
            row_height = HEIGHT;
            row_width = WIDTH;
            sign = -1 * sign;
        }
    }

cleanup:
    if (SDL_MUSTLOCK(surface)) {
        SDL_UnlockSurface(surface);
    }

    //SDL_UpdateWindowSurface(window);

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
