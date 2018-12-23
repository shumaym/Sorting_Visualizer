#ifndef __VISUALS_H__
#define __VISUALS_H__

#include "../globals.h"

void create_bars(
		std::vector<uint16_t>& elems,
		uint16_t num_elems,
		SDL_Rect bars[]);

void draw_bars(
		uint16_t num_elems,
		SDL_Renderer *renderer,
		SDL_Rect bars[],
		std::vector<uint16_t>& elems_accessed);

void create_frame(
		std::vector<uint16_t>& elems,
		uint16_t num_elems,
		SDL_Renderer *renderer,
		SDL_Rect bars[],
		std::vector<uint16_t>& elems_accessed);

#endif