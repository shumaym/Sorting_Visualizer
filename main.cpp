#include <SDL2/SDL.h>

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <math.h>
#include <signal.h>
#include <getopt.h>
#include <sstream>

#include "globals.h"
#include "headers/visuals.h"
#include "headers/algorithms.h"

const option long_opts[] = {
	{"frame-delay", 1, 0, 'd'},
	{"dimensions", 1, 0, 'z'},
	{"help", 0, 0, 'h'},
	{"sorting", 1, 0, 's'},
	{0,0,0,0}
};

const std::string sort_method_names[7] = {
	"bubble sort",
	"selection sort",
	"insertion sort",
	"quicksort",
	"mergesort",
	"heapsort",
	"introsort"};

// Screen properties.
uint16_t screen_width = 1500;
uint16_t screen_height = 1000;
const float SCREEN_MARGINS = 0.05;
const float BAR_SEPARATION = 0.2;

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
				std::cout << "    5: heapsort" << std::endl;
				std::cout << "    6: heapsort" << std::endl;

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
				if (std::atoi(optarg) >= 0 && std::atoi(optarg) < 7){
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
		case 5:
			heapsort(elems, 0, num_elems, num_elems, bars, renderer);
			break;
		case 6:
			int depth = std::floor(log(num_elems)) * 2;
			introsort(elems, depth, 0, num_elems - 1, num_elems, bars, renderer);
			break;
	}

	// Show sorted list
	create_bars(elems, num_elems, bars);
	std::vector<uint16_t> elems_accessed;
	draw_bars(num_elems, renderer, bars, elems_accessed);

	if (check_sorted(elems, num_elems))
		std::cout << "Sorted!" << std::endl;
	else
		std::cout << "Sorting Failure!" << std::endl;
		

	// Pauses after sorting is complete
	SDL_Delay(2000);
}