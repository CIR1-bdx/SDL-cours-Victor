#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sv_reader.c"

#define WINDOWHEIGHT 900
#define WINDOWWIDTH 1200
#define NAMEWINDOW "SDL Cours de victor"
#define FONTPATHPAGE "./Roboto-Regular.ttf"
#define FONTSIZETITLE 24
#define FONTSIZECONTENT 16


int nbPage = 10;

typedef struct Title{
    char title[1024];
    int y;
    SDL_Surface *surface_text;
    SDL_Texture *texture_text;
    SDL_Rect renderQuad;
}Title;

typedef struct ContentText{
    char contentText[2028];
    int y;
    int x;
    SDL_Surface *surface_text;
    SDL_Texture *texture_text;
    SDL_Rect renderQuad;
}ContentText;

typedef struct PathImage{
    char pathImage[1024];
    int y;
    int x;
    SDL_Texture *texture_image;
}PathImage;

typedef struct Link{
    char link[1024];
}Link;

typedef struct Page
{
    Title title;
    ContentText contentText;
    PathImage pathImage;
    Link link;
    SDL_Rect Buttons[5];
}Page;

Page page;





void drawTitle();

void drawText(SDL_Renderer *renderer, int indexPage)
{
    SDL_RenderCopy(renderer, page.contentText.texture_text, NULL, &page.contentText.renderQuad);
}

void drawImage();

void drawButtons();
void completeRenderer();
void drawHomeButton();

SDL_Rect principalPageButton;

void principalPage(SDL_Renderer *renderer)
{
    int x = 50;
    int y = 100;
    for (int i = 1; i < nbPage+1; i++)
    {
        principalPageButton.x = x;
        principalPageButton.y = y;
        principalPageButton.w = 200;
        principalPageButton.h = 80;

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &principalPageButton);

        TTF_Font *font = TTF_OpenFont(FONTPATHPAGE, FONTSIZECONTENT);
        SDL_Color textColor = {0, 0, 0, 255};
        SDL_Surface *surface = TTF_RenderText_Solid(font, "TITLE", textColor);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

        int text_width = surface->w;
        int text_height = surface->h;
        SDL_FreeSurface(surface);

        SDL_Rect renderQuad = {x + 20, principalPageButton.y + text_height, text_width, text_height};
        SDL_RenderCopy(renderer, texture, NULL, &renderQuad);

        if (i%4 == 0) { y += 200; x = 50;}
        else {x += 300;}
    }

}


void contentMain(SDL_Renderer *renderer)
{
    SDL_RenderClear(renderer);

    principalPage(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderPresent(renderer);
}

int initializePages(SDL_Renderer *renderer, Page *pages[], SDL_Color color, TTF_Font *fontTitle, TTF_Font *fontContent)
{
    for (int i = 0; i < nbPage; i++)
    {
        pages[i]->title.surface_text = TTF_RenderText_Solid(fontTitle, pages[i]->title.title, color);
        pages[i]->title.texture_text = SDL_CreateTextureFromSurface(renderer, pages[i]->title.surface_text);
        SDL_Rect renderQuad = {WINDOWWIDTH/2 - pages[i]->title.surface_text->w/2, pages[i]->title.y, pages[i]->title.surface_text->w, pages[i]->title.surface_text->h};

        pages[i]->contentText.surface_text = TTF_RenderText_Solid(fontContent, pages[i]->contentText.contentText, color);
        pages[i]->contentText.texture_text = SDL_CreateTextureFromSurface(renderer, pages[i]->contentText.surface_text);
        SDL_Rect renderQuad = {pages[i]->contentText.x, pages[i]->contentText.y, pages[i]->contentText.surface_text->w, pages[i]->contentText.surface_text->h};
        pages[i]->contentText.renderQuad = renderQuad;

        SDL_Surface *surface = IMG_Load(pages[i]->pathImage.pathImage);
        pages[i]->pathImage.texture_image = SDL_CreateTextureFromSurface(renderer, surface);
    }
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
    TTF_Font *fontTitle = TTF_OpenFont(FONTPATHPAGE, FONTSIZETITLE); // Modify the path and size
    if (!fontTitle)
    {
        SDL_Log("TTF_OpenFont: %s\n", TTF_GetError());
    }
    TTF_Font *fontContent = TTF_OpenFont(FONTPATHPAGE, FONTSIZECONTENT); // Modify the path and size
    if (!fontContent)
    {
        SDL_Log("TTF_OpenFont: %s\n", TTF_GetError());
    }
    SDL_Color textColor = {0, 0, 0, 255}; // Black color

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

    int indexPage = 0;

    SDL_Event event;
    int running = 1;

    Page pages[1024];
    read_pages(pages);
    initializePages(renderer, &pages, textColor, fontTitle, fontContent);


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
        SDL_Delay(1000 / 1000);
    }

    // Cleanup
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}