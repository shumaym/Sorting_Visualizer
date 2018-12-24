#include "../headers/visuals.h"

/**
* Compute the dimensions of all bars, each representing an element in the list.
*/
void create_bars(std::vector<uint16_t>& elems){
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
* Draw to the screen the element bars, using the appropriate colours.
*/
void draw_bars(){
	// Draw all bars in white.
	SDL_SetRenderDrawColor(renderer, COLOUR_BARS_WHITE[0], COLOUR_BARS_WHITE[1], COLOUR_BARS_WHITE[2], 0xFF);
	SDL_RenderFillRects(renderer, bars, num_elems);

	// Draw accessed elements' bars again, but in red.
	SDL_SetRenderDrawColor(renderer, COLOUR_BARS_RED[0], COLOUR_BARS_RED[1], COLOUR_BARS_RED[2], 0xFF);
	for (uint16_t i = 0; i < elems_accessed.size(); i++)
		SDL_RenderFillRect(renderer, &bars[elems_accessed[i]]);
}

/**
* Draw to the screen the text specifying the number of comparisons and swaps.
*/
void draw_text(){
	SDL_Color text_colour = {255, 255, 255};
	SDL_Color bg_colour= {0, 0, 0};
	std::string text_top = "Comparisons: " + std::to_string(num_comps);
	text_top += "    Swaps: " + std::to_string(num_swaps);
	SDL_Surface *text_surface = TTF_RenderText_Shaded(g_font, text_top.c_str(), text_colour, bg_colour);

	int x = screen_width * SCREEN_MARGINS;
	int y = screen_height * SCREEN_MARGINS * 0.15;

	int text_width = text_surface->w;
	int text_height = text_surface->h;

	SDL_Rect text_rect = {x, y, text_width, text_height};
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

}

/**
* Manages the creation and displaying of a new visual frame.
*/
void create_frame(std::vector<uint16_t>& elems){
	// Clear the renderer.
	SDL_SetRenderDrawColor(renderer, COLOUR_BACKGROUND[0], COLOUR_BACKGROUND[1], COLOUR_BACKGROUND[2], 0xFF);
	SDL_RenderClear(renderer);

	create_bars(elems);
	draw_bars();
	draw_text();

	// Update the screen.
	SDL_RenderPresent(renderer);
	elems_accessed.clear();
	SDL_Delay(frame_delay_ms);
}