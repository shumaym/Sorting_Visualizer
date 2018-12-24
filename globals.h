#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <vector>
#include <random>
#include <algorithm>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

extern uint16_t screen_width;
extern uint16_t screen_height;
extern const float SCREEN_MARGINS;
extern const float BAR_SEPARATION;

extern const uint8_t COLOUR_BACKGROUND[3];
extern const uint8_t COLOUR_BARS_WHITE[3];
extern const uint8_t COLOUR_BARS_RED[3];

extern uint16_t frame_delay_ms;
extern uint16_t num_elems;

extern std::mt19937 generator;

extern std::vector<uint16_t> elems;
extern std::vector<uint16_t> elems_accessed;
extern SDL_Rect *bars;

extern uint32_t num_comps;
extern uint32_t num_swaps;

extern SDL_Renderer *renderer;
extern TTF_Font *g_font;
extern SDL_Surface *text_surface;
extern SDL_Texture *text_texture;
extern SDL_Event event;


#endif