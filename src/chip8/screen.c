#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "screen.h"


static SDL_Surface* screen = NULL;
static uint8_t scr_width, scr_height;

uint8_t* init_screen(uint8_t width, uint8_t height)
{
    scr_width = width;
    scr_height = height;
    uint8_t* buffer = (uint8_t*)malloc(width*height*sizeof(uint8_t));

    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Error: Failed to initialize SDL.\n");
    }
    screen = SDL_SetVideoMode(
            scr_width*PIXEL_SIZE,
            scr_height*PIXEL_SIZE,
            0,
            SDL_HWSURFACE | SDL_DOUBLEBUF);

    SDL_WM_SetCaption("chip8", 0);
    return buffer;
}

void display_draw(uint8_t* buffer)
{
    for (uint8_t y=0; y < scr_height; y++)
    {
        for (uint8_t x=0; x < scr_width; x++)
        {
            if ((buffer[x + y*scr_width] & 0x01) == 0x01)
                boxRGBA(screen,
                        x*PIXEL_SIZE, y*PIXEL_SIZE,
                        (x+1)*PIXEL_SIZE, (y+1)*PIXEL_SIZE,
                        0xFF, 0xFF, 0xFF, 0xFF);
            else
                boxRGBA(screen,
                        x*PIXEL_SIZE, y*PIXEL_SIZE,
                        (x+1)*PIXEL_SIZE, (y+1)*PIXEL_SIZE,
                        0x00, 0x00, 0x00, 0xFF);
        }
    }
    SDL_Flip(screen);
}

void clear_buffer(uint8_t* buffer)
{
    for(int i = 0; i < scr_width*scr_height; i++)
        *(buffer + i) = 0;
}

void end_screen(uint8_t* buffer)
{
    SDL_FreeSurface(screen);
    free(buffer);
    SDL_Quit();
}
