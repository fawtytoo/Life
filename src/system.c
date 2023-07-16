//  life

#include <SDL2/SDL.h>

#include "common.h"

SDL_Window  *sdl_Window;
SDL_Surface *sdl_Surface;
SDL_Rect    sdl_Rect;

Uint32      sdl_Colour[2] =
{
    0x00000000,
    0x00ffffff
};

void SystemDraw(int x, int y, int colour, int inset)
{
    x *= CELLSIZE;
    y *= CELLSIZE;
    sdl_Rect.x = x + inset;
    sdl_Rect.y = y + inset;
    sdl_Rect.w = sdl_Rect.h = CELLSIZE - inset * 2;

    SDL_FillRect(sdl_Surface, &sdl_Rect, sdl_Colour[colour]);
}

int SystemTicks()
{
    static int  then = 0;
    int         now, diff;

    now = SDL_GetTicks();
    diff = now - then;
    then = now;

    return diff;
}

int SystemEvent(int *key)
{
    static int  mouse_x, mouse_y;
    SDL_Event   event;

    *key = KEY_NONE;

    if (SDL_PollEvent(&event) == 0)
        return 0;

    switch (event.type)
    {
      case SDL_MOUSEBUTTONDOWN:
        WorldCellPosChange(mouse_x, mouse_y);
        return 1;

      case SDL_MOUSEMOTION:
        mouse_x = event.motion.x / CELLSIZE;
        mouse_y = event.motion.y / CELLSIZE;
        return 1;

      case SDL_KEYUP:
        break;

      case SDL_QUIT:
        *key = KEY_QUIT;
        return 1;

      default:
        return 1;
    }

    if (event.key.repeat)
        return 1;

    switch (event.key.keysym.sym)
    {
      case SDLK_LEFT:
        *key = KEY_LEFT;
        break;

      case SDLK_RIGHT:
        *key = KEY_RIGHT;
        break;

      case SDLK_UP:
        *key = KEY_UP;
        break;

      case SDLK_DOWN:
        *key = KEY_DOWN;
        break;

      case SDLK_SPACE:
        *key = KEY_SPACE;
        break;

      case SDLK_RETURN:
        *key = KEY_START;
        break;

      case SDLK_ESCAPE:
        *key = KEY_STOP;
        break;

      case SDLK_BACKSPACE:
        *key = KEY_CLEAR;
        break;

      default:
        *key = KEY_ANY;
        break;
    }

    return 1;
}

void SystemUpdate()
{
    SDL_UpdateWindowSurface(sdl_Window);
    SDL_Delay(1);
}

void SystemQuit()
{
    SDL_DestroyWindow(sdl_Window);

    SDL_Quit();
}

void SystemInit()
{
    SDL_Init(SDL_INIT_VIDEO);

    sdl_Window = SDL_CreateWindow("Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH * CELLSIZE, HEIGHT * CELLSIZE, 0);
    sdl_Surface = SDL_GetWindowSurface(sdl_Window);
}

//  life
