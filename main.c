#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WINDOWHEIGHT 900
#define WINDOWWIDTH 1200
#define NAMEWINDOW "SDL Cours de victor"
#define FONTPATHPAGE "./Roboto-Regular.ttf"

typedef struct Title{
    char title[1024];
    int y;
}Title;

typedef struct ContentText{
    char contentText[1024];
    int y;
    int x;
}ContentText;

typedef struct PathImage{
    char contentText[1024];
    int y;
    int x;
}PathImage;

typedef struct Page
{
    Title title;
    ContentText contentText;
    PathImage pathImage;
    SDL_Rect Button[5];
}Page;

void contentMain(SDL_Renderer *renderer)
{
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }
    if (TTF_Init() == -1)
    {
        SDL_Log("TTF_Init: %s\n", TTF_GetError());
    }
    TTF_Font *font = TTF_OpenFont(FONTPATHPAGE, 24); // Modify the path and size
    if (!font)
    {
        SDL_Log("TTF_OpenFont: %s\n", TTF_GetError());
    }

    // Create a window
    SDL_Window *window = SDL_CreateWindow(NAMEWINDOW,
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          WINDOWWIDTH, WINDOWHEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        SDL_Log("Unable to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }


    SDL_Event event;
    int running = 1;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = 0;
                break;
            }
        }
        contentMain(renderer);
    }

    // Cleanup
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}