#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WINDOWHEIGHT 900
#define WINDOWWIDTH 1200
#define NAMEWINDOW "SDL Cours de victor"
#define FONTPATHPAGE "./Roboto-Regular.ttf"
#define FONTSIZETITLE 24
#define FONTSIZECONTENT 16

int nbPage = 0;

typedef struct Title{
    char title[1024];
    int y = 80 ;
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

void drawTitle(SDL_Renderer* renderer, Title* titleStruct) {
    TTF_Font* font = TTF_OpenFont("Baloo-Regular.ttf", 40);
    if (font == NULL) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return;
    }

    SDL_Color textColor = {255, 255, 255};
    SDL_Color bgColor = {0, 0, 0};
    SDL_Surface* surface = TTF_RenderText_Shaded(font, titleStruct->title, textColor, bgColor);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    int textWidth = surface->w;
    int textHeight = surface->h;

    SDL_FreeSurface(surface);

    SDL_GetRendererOutputSize(renderer, &WINDOWWIDTH, &WINDOWHEIGHT);

    SDL_Rect renderQuad = {(WINDOWWIDTH - textWidth) / 2, titleStruct->y , textWidth, textHeight};
    SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}

void drawText(Page page, SDL_Renderer *rend){
    char* txt = page.contentText.contentText;
    TTF_Font *font = TTF_OpenFont(FONTPATHPAGE, FONTSIZECONTENT);
    SDL_Color textColor = {255, 255, 255, 255};

    SDL_Surface *surface_text = TTF_RenderText_Solid(font, txt, textColor);
    SDL_Texture *texture_text = SDL_CreateTextureFromSurface(rend, surface_text);

    int text_width = surface_text->w;
    int text_height = surface_text->h;
    SDL_FreeSurface(surface_text);

    SDL_Rect renderQuad_text = {100, 300, text_width, text_height};
    SDL_RenderCopy(rend, texture_text, NULL, &renderQuad_text);

    SDL_DestroyTexture(texture_text);
    TTF_CloseFont(font);
}

void drawImage();

void drawButtons();
void completeRenderer();
void drawHomeButton();

SDL_Rect principalPageButton;

void principalPage(SDL_Renderer *renderer)
{
    int x = 20;
    int y = 50;
    for (int i = 1; i < nbPage+1; i++)
    {
        principalPageButton.x = x;
        principalPageButton.y = y;
        principalPageButton.w = 150;
        principalPageButton.h = 80;

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &principalPageButton);

        if (i%6 == 0) { y += 200; x = 20;}
        else {x += 200;}
    }
}

void contentMain(SDL_Renderer *renderer)
{
    int a = 1;
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    if (a == 0)
    {
        principalPage(renderer);
    }

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
