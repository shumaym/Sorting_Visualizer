#include <SDL2/SDL.h>

#include <iostream>
#include <array>
#include <vector>
#include <numeric>
#include <algorithm>
#include <utility>
#include <random>
#include <signal.h>
#include <unistd.h>
#include <getopt.h>
#include <sstream>

const option long_opts[] = {
	{"frame-delay", 1, 0, 'd'},
	{"dimensions", 1, 0, 'z'},
	{"help", 0, 0, 'h'},
	{"sorting", 1, 0, 's'},
	{0,0,0,0}
};

const std::string sort_method_names[5] = {"bubble sort", "selection sort", "insertion sort", "quicksort", "mergesort"};

// Screen properties.
uint16_t screen_width = 1500;
uint16_t screen_height = 1000;
const float SCREEN_MARGINS = 0.05;

// RGB colour values.
const uint8_t COLOUR_BACKGROUND[3] = {0x00, 0x00, 0x00};
const uint8_t COLOUR_BARS_WHITE[3] = {0xFF, 0xFF, 0xFF};
const uint8_t COLOUR_BARS_RED[3] = {0xFF, 0x00, 0x00};

// Default parameters.
uint16_t num_elems = 50;
uint16_t frame_delay_ms = 50;
uint16_t sort_method = 3;

std::random_device random_dev;
std::mt19937 generator(random_dev());

/**
* Log an SDL error with some error message
* @param msg: The error message to write, format will be msg error: SDL_GetError()
*/
void log_SDL_error(const std::string &msg){
	std::cerr << msg << " error: " << SDL_GetError() << std::endl;
}

/**
* Handles interrupts, sets a flag to discontinue computation.
*/
void signal_interrupt(int _){
	std::cout << std::endl << "Exiting." << std::endl;
	exit(0);
}

/**
* Prints out each element of a vector, separated by a newline.
*/
void print_elems(std::vector<uint16_t>& elems, uint16_t num_elems){
	for(int i = 0; i < num_elems; i++)
		std::cout << elems[i] << std::endl;
	std::cout << std::endl;
}

/**
* Compute the dimensions of all bars, each representing an element in the list.
*/
void create_bars(
		std::vector<uint16_t>& elems,
		uint16_t num_elems,
		SDL_Rect bars[]){

	float bar_section_width = screen_width * (1 - 2 * SCREEN_MARGINS) / num_elems;
	float bar_separation = 0.2;
	uint16_t bar_width = bar_section_width * (1 - bar_separation);
	
	int bar_idx;
	int x, y, bar_height;
	for(bar_idx = 0; bar_idx < num_elems; bar_idx++){
		// Calculate top-left corner's x coordinate.
		x = screen_width * SCREEN_MARGINS;
		x += (bar_section_width * bar_idx);
		x += (bar_section_width * bar_separation / 2);

		// Calculate top-left corner's y coordinate.
		y = screen_height * (1 - SCREEN_MARGINS) - float(elems[bar_idx]) / num_elems * screen_height * (1 - 2 * SCREEN_MARGINS);

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
		std::vector<uint16_t> elems_accessed){

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

/**
* Checks if a vector contains a sorted sequence of elements.
*/
int check_sorted(std::vector<uint16_t>& elems, uint16_t num_elems){
	int last_elem = elems[0];
	for (int i = 1; i < num_elems; i++){
		if (elems[i] < last_elem)
			return 0;
		last_elem = elems[i];
	}
	return 1;
}

/**
* A spectacularly inefficient sorting method, averages O((n+1)!) time.
*/
void bogo_sort(
		std::vector<uint16_t>& elems,
		uint16_t num_elems,
		SDL_Rect bars[],
		SDL_Renderer *renderer){

	bool flag_sorted = false;
	std::vector<uint16_t> elems_accessed;	
	
	while (!flag_sorted){
		std::shuffle(elems.begin(), elems.end(), generator);
		create_bars(elems, num_elems, bars);
		draw_bars(num_elems, renderer, bars, elems_accessed);
		SDL_Delay(frame_delay_ms);
		flag_sorted = check_sorted(elems, num_elems);
	}
}

/**
* Bubble sort, averages O(n^2) time.
*/
void bubble_sort(
		std::vector<uint16_t>& elems,
		uint16_t num_elems,
		SDL_Rect bars[],
		SDL_Renderer *renderer){
	std::vector<uint16_t> elems_accessed;	

	int i, j;
	bool sorted = false;
	for (i = 0; i < num_elems - 1 && !sorted; i++){
		sorted = true;
		for (j = 0; j < num_elems - i - 1; j++){
			if (elems[j] > elems[j + 1]){
				std::swap(elems[j], elems[j + 1]);
				sorted = false;
			}
			
			// Visualization code below.
			elems_accessed.push_back(j);
			elems_accessed.push_back(j + 1);
			create_bars(elems, num_elems, bars);
			draw_bars(num_elems, renderer, bars, elems_accessed);
			elems_accessed.clear();
			SDL_Delay(frame_delay_ms);
		}
	}
}

/**
* Selection sort, averages O(n^2) time.
*/
void selection_sort(
		std::vector<uint16_t>& elems,
		uint16_t num_elems,
		SDL_Rect bars[],
		SDL_Renderer *renderer){

	std::vector<uint16_t> elems_accessed;
	int i, j, min;
	int n = elems.size();

	for (i = 0; i < n - 1; i++){
		min = i;
		for (j = i + 1; j < n; j++){
			if (elems[j] < elems[min])
				min = j;

			// Visualization code below.
			elems_accessed.push_back(min);
			elems_accessed.push_back(j);
			create_bars(elems, num_elems, bars);
			draw_bars(num_elems, renderer, bars, elems_accessed);
			elems_accessed.clear();
			SDL_Delay(frame_delay_ms);
		}
		if (min != i){
			std::swap(elems[min], elems[i]);

			// Visualization code below.
			elems_accessed.push_back(min);
			elems_accessed.push_back(i);
			create_bars(elems, num_elems, bars);
			draw_bars(num_elems, renderer, bars, elems_accessed);
			elems_accessed.clear();
			SDL_Delay(frame_delay_ms);
		}
	}
}

/**
* Insertion sort, averages O(n^2) time.
*/
void insertion_sort(
		std::vector<uint16_t>& elems,
		uint16_t num_elems,
		SDL_Rect bars[],
		SDL_Renderer *renderer){

	std::vector<uint16_t> elems_accessed;
	int i, j;
	for (i = 1; i < elems.size(); i++){
		for (j = i; j > 0 && elems[j - 1] > elems[j]; j--){
			std::swap(elems[j], elems[j - 1]);

			// Visualization code below.
			elems_accessed.push_back(j);
			elems_accessed.push_back(j - 1);
			create_bars(elems, num_elems, bars);
			draw_bars(num_elems, renderer, bars, elems_accessed);
			elems_accessed.clear();
			SDL_Delay(frame_delay_ms);
		}
	}
}

/**
* Partition function of quicksort.
*/
uint16_t quicksort_partition(
		std::vector<uint16_t>& elems,
		uint16_t num_elems,
		uint16_t start_idx,
		uint16_t end_idx,
		SDL_Rect bars[],
		SDL_Renderer *renderer){
	std::vector<uint16_t> elems_accessed;

	// Set the pivot to the median of the first, middle, and last elements.
	int mid_idx = (start_idx + end_idx) / 2;
	if (elems[mid_idx] < elems[start_idx]);
		std::swap(elems[start_idx], elems[mid_idx]);
	if (elems[end_idx] < elems[start_idx]);
		std::swap(elems[start_idx], elems[end_idx]);
	if (elems[mid_idx] < elems[end_idx]);
		std::swap(elems[mid_idx], elems[end_idx]);
	int pivot = elems[end_idx];

	int i = start_idx;
	for (int j = start_idx; j < end_idx; j++){
		if (elems[j] < pivot){
			elems_accessed.push_back(i);
			if (i != j){
				std::swap(elems[j], elems[i]);
			}
			i++;
		}

		// Visualization code below.
		elems_accessed.push_back(end_idx);
		elems_accessed.push_back(j);
		create_bars(elems, num_elems, bars);
		draw_bars(num_elems, renderer, bars, elems_accessed);
		elems_accessed.clear();
		SDL_Delay(frame_delay_ms);
	}
	std::swap(elems[i], elems[end_idx]);
	
	// Visualization code below.
	elems_accessed.push_back(i);
	elems_accessed.push_back(end_idx);
	create_bars(elems, num_elems, bars);
	draw_bars(num_elems, renderer, bars, elems_accessed);
	elems_accessed.clear();
	SDL_Delay(frame_delay_ms);
	
	return i;
}

/**
* Quicksort, averages O(n*log(n)) time.
*/
void quicksort(
		std::vector<uint16_t>& elems,
		uint16_t num_elems,
		int start_idx,
		int end_idx,
		SDL_Rect bars[],
		SDL_Renderer *renderer){

	if (start_idx < end_idx){
		int pivot = quicksort_partition(elems, num_elems, start_idx, end_idx, bars, renderer);
		quicksort(elems, num_elems, start_idx, pivot - 1, bars, renderer);
		quicksort(elems, num_elems, pivot + 1, end_idx, bars, renderer);
	}
}

/**
* Merge function of mergesort.
*/
void bottom_up_merge(
		std::vector<uint16_t>& A,
		uint16_t left_idx,
		uint16_t right_idx,
		uint16_t end_idx,
		std::vector<uint16_t>& B,
		uint16_t num_elems,
		SDL_Rect bars[],
		SDL_Renderer *renderer){

	std::vector<uint16_t> elems_accessed;
	int i = left_idx;
	int j = right_idx;
	for (int k = left_idx; k < end_idx; k++){
		if (i < right_idx && (j >= end_idx || A[i] <= A[j])){
			B[k] = A[i];
			i++;
		} else {
			B[k] = A[j];
			j++;
		}

		// Visualization code below.
		elems_accessed.push_back(i);
		elems_accessed.push_back(j);
		elems_accessed.push_back(k);
		create_bars(B, num_elems, bars);
		draw_bars(num_elems, renderer, bars, elems_accessed);
		elems_accessed.clear();
		SDL_Delay(frame_delay_ms);

	}
}

/**
* Mergesort, averages O(n*log(n)) time.
*/
void bottom_up_mergesort(
		std::vector<uint16_t>& A,
		uint16_t num_elems,
		SDL_Rect bars[],
		SDL_Renderer *renderer){

	std::vector<uint16_t> B(A);
	for (int width = 1; width < num_elems; width = 2 * width){
		for (int i = 0; i < num_elems; i = i + 2 * width)
			bottom_up_merge(A, i, std::min(i+width, (int) num_elems), std::min(i+2*width, (int) num_elems), B, num_elems, bars, renderer);
		// Copy B into A
		A = B;
	}
}

int main(int argc, char *argv[]){
	signal(SIGINT, signal_interrupt);

	// Process passed arguments
	int opt, prev_ind;
	while(prev_ind = optind, (opt = getopt_long(argc, argv, "hn:d:s:", long_opts, &optind)) != EOF){
		switch(opt){
			case 'h':
				std::cout << std::endl << "Options:" << std::endl;
				std::cout << " -n N                        number of elements to sort (default: " << num_elems << ")" << std::endl;
				std::cout << " -d N, --frame-delay N       delay in ms after the window is refreshed (default: " << frame_delay_ms << ")" << std::endl;
				std::cout << " --dimensions XxY            screen dimensions (default: " << screen_width << "x" << screen_height << ")" << std::endl;
				std::cout << " -h, --help                  display this help page and exit" << std::endl;
				std::cout << " -s N, sorting N             sorting method (default: quicksort)" << std::endl;
				std::cout << "    0: bubble sort" << std::endl;
				std::cout << "    1: selection sort" << std::endl;
				std::cout << "    2: insertion sort" << std::endl;
				std::cout << "    3: quicksort" << std::endl;
				std::cout << "    4: mergesort" << std::endl;

				std::cout << std::endl << std::endl;
				return 0;

			case 'd':
				if (std::atoi(optarg) >= 0){
					frame_delay_ms = std::atoi(optarg);
					std::cout << "Frame delay set to " << frame_delay_ms << " ms." << std::endl;
				}
				else
					std::cout << "Invalid frame delay. Defaulting to " << frame_delay_ms << " ms." << std::endl;
				break;

			case 'n':
				if (std::atoi(optarg) >= 2 && std::atoi(optarg) <= pow(2, 16)){
					num_elems = std::atoi(optarg);
					std::cout << "Number of elements set to " << num_elems << "." << std::endl;
				}
				else
					std::cout << "Invalid number of elements. Defaulting to " << num_elems << "." << std::endl;
				break;

			case 's':
				if (std::atoi(optarg) >= 0 && std::atoi(optarg) <= 4){
					sort_method = std::atoi(optarg);
					std::cout << "Sorting method set to " << sort_method_names[sort_method] << "." << std::endl;
				}
				else
					std::cout << "Invalid sorting method. Defaulting to " << sort_method_names[sort_method] << "." << std::endl;
				break;

			case 'z':
				// case for "dimensions" option, gathers dimensions from optarg in form "XxY".
				std::stringstream ss(optarg);
				std::string item;
				std::vector<int> split_strings;
				int num_items = 0;
				while (std::getline(ss, item, 'x')){
					split_strings.push_back(uint16_t (std::atoi(item.c_str())));
					num_items++;
				}
				if (num_items != 2 || split_strings[0] <= 0 || split_strings[1] <= 0)
					std::cout << "Invalid screen dimensions. Defaulting to " << screen_width << "x" << screen_height << std::endl;
				else{
					screen_width = split_strings[0];
					screen_height = split_strings[1];
				}
				break;
		}
	}

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		log_SDL_error("SDL_Init");
		return 1;
	}

	// Create window
	SDL_Window *window = SDL_CreateWindow("Sorting Visualizer",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		screen_width, screen_height,
		0);
	if (window == nullptr){
		log_SDL_error("CreateWindow");
		SDL_Quit();
		return 1;
	}

	// Create renderer
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);	
	if (renderer == nullptr){
		log_SDL_error("CreateRenderer");
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	// Create and shuffle (num_elems) elements
	std::vector<uint16_t> elems(num_elems);
	std::iota(std::begin(elems), std::end(elems), 1);
	std::shuffle(elems.begin(), elems.end(), generator);

	// Create array for the bars
	SDL_Rect bars[num_elems];

	// Sort using the specified/default sort method
	switch(sort_method){
		case 0:
			bubble_sort(elems, num_elems, bars, renderer);
			break;
		case 1:
			selection_sort(elems, num_elems, bars, renderer);
			break;
		case 2:
			insertion_sort(elems, num_elems, bars, renderer);
			break;
		case 3:
			quicksort(elems, num_elems, 0, num_elems - 1, bars, renderer);
			break;
		case 4:
			bottom_up_mergesort(elems, num_elems, bars, renderer);
			break;
	}

	// Show sorted list
	create_bars(elems, num_elems, bars);
	std::vector<uint16_t> elems_accessed;
	draw_bars(num_elems, renderer, bars, elems_accessed);
	// Pauses after sorting is complete
	SDL_Delay(2000);
}