#include <SDL2/SDL.h>
#include <vector>

#include "../headers/visuals.h"

/**
* Compute the dimensions of all bars, each representing an element in the list.
*/
void create_bars(
		std::vector<uint16_t>& elems,
		uint16_t num_elems,
		SDL_Rect bars[]){

	float bar_section_width = screen_width * (1 - 2 * SCREEN_MARGINS) / num_elems;
	uint16_t bar_width = bar_section_width * (1 - BAR_SEPARATION);
	
	int bar_idx;
	int x, y, bar_height;
	for(bar_idx = 0; bar_idx < num_elems; bar_idx++){
		// Calculate top-left corner's x coordinate.
		x = screen_width * SCREEN_MARGINS;
		x += (bar_section_width * bar_idx);
		x += (bar_section_width * BAR_SEPARATION / 2);

		// Calculate top-left corner's y coordinate.
		y = screen_height * (1 - SCREEN_MARGINS);
		y -= float(elems[bar_idx]) / num_elems * screen_height * (1 - 2 * SCREEN_MARGINS);

		// Calculate the height of the bar.
		bar_height = screen_height * (1 - SCREEN_MARGINS) - y;

		// Create an SDL_Rect and insert it into the (bars) array.
		bars[bar_idx] = SDL_Rect{x, y, bar_width, bar_height};
	}
}

/**
* Draw the element bars to the screen, using the appropriate colours.
*/
void draw_bars(
		uint16_t num_elems,
		SDL_Renderer *renderer,
		SDL_Rect bars[],
		std::vector<uint16_t>& elems_accessed){

	// Clear the renderer.
	SDL_SetRenderDrawColor(renderer, COLOUR_BACKGROUND[0], COLOUR_BACKGROUND[1], COLOUR_BACKGROUND[2], 0xFF);
	SDL_RenderClear(renderer);

	// Draw all bars in white.
	SDL_SetRenderDrawColor(renderer, COLOUR_BARS_WHITE[0], COLOUR_BARS_WHITE[1], COLOUR_BARS_WHITE[2], 0xFF);
	SDL_RenderFillRects(renderer, bars, num_elems);

	// Draw accessed elements' bars again, but in red.
	SDL_SetRenderDrawColor(renderer, COLOUR_BARS_RED[0], COLOUR_BARS_RED[1], COLOUR_BARS_RED[2], 0xFF);
	for (int i = 0; i < elems_accessed.size(); i++){
		SDL_RenderFillRect(renderer, &bars[elems_accessed[i]]);
	}

	// Update the screen.
	SDL_RenderPresent(renderer);
}

void create_frame(
		std::vector<uint16_t>& elems,
		uint16_t num_elems,
		SDL_Renderer *renderer,
		SDL_Rect bars[],
		std::vector<uint16_t>& elems_accessed){

	create_bars(elems, num_elems, bars);
	draw_bars(num_elems, renderer, bars, elems_accessed);
	elems_accessed.clear();
	SDL_Delay(frame_delay_ms);
}