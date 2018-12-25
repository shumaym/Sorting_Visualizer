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
const float SCREEN_MARGINS = 0.075;
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

// Stores the list of elements to sort
std::vector<uint16_t> elems;
// Tracks which indices of (elems) have been accessed in a period of operations
std::vector<uint16_t> elems_accessed;
// Holds the rectangles that visually represent the elements in (elems)
SDL_Rect *bars;

// Records the number of element comparisons
uint32_t num_comps = 0;
// Records the number of element swaps
uint32_t num_swaps = 0;

SDL_Window *window;
SDL_Renderer *renderer;
TTF_Font *g_font;
SDL_Surface *text_surface;
SDL_Texture *text_texture;
SDL_Event event;

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
	std::cout << std::endl << "Exiting\n";
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
* Load in the ttf font that will be used for text output.
*/
void load_font(){
	int font_size = screen_height * SCREEN_MARGINS / 3;
	g_font = TTF_OpenFont("fonts/Roboto/Roboto-Regular.ttf", font_size);
}

/**
* Called on exit, Frees and destroys.
*/
void exit_function(){
	std::vector<uint16_t>().swap(elems);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

int main(int argc, char *argv[]){
	signal(SIGINT, signal_interrupt);
	atexit(exit_function);

	// Process passed arguments
	int opt, prev_ind;
	while(prev_ind = optind, (opt = getopt_long(argc, argv, "hn:d:s:", long_opts, &optind)) != EOF){
		switch(opt){
			case 'h':
				std::cout << std::endl << "Options:\n";
				std::cout << " -n N                        number of elements to sort (default: " << num_elems << ")\n";
				std::cout << " -d N, --frame-delay N       delay in ms after the window is refreshed (default: " << frame_delay_ms << ")\n";
				std::cout << " --dimensions XxY            screen dimensions (default: " << screen_width << "x" << screen_height << ")\n";
				std::cout << " -h, --help                  display this help page and exit\n";
				std::cout << " -s N, sorting N             sorting method (default: quicksort)\n";
				std::cout << "    0: bubble sort\n";
				std::cout << "    1: selection sort\n";
				std::cout << "    2: insertion sort\n";
				std::cout << "    3: quicksort\n";
				std::cout << "    4: mergesort\n";
				std::cout << "    5: heapsort\n";
				std::cout << "    6: introsort\n";
				std::cout << std::endl << std::endl;
				return 0;

			case 'd':
				if (std::atoi(optarg) >= 0){
					frame_delay_ms = std::atoi(optarg);
					std::cout << "Frame delay set to " << frame_delay_ms << " ms." << std::endl;
				}
				else
					std::cerr << "Invalid frame delay. Defaulting to " << frame_delay_ms << " ms." << std::endl;
				break;

			case 'n':
				if (std::atoi(optarg) >= 2 && std::atoi(optarg) <= pow(2, 16)){
					num_elems = std::atoi(optarg);
					std::cout << "Number of elements set to " << num_elems << "." << std::endl;
				}
				else
					std::cerr << "Invalid number of elements. Defaulting to " << num_elems << "." << std::endl;
				break;

			case 's':
				if (std::atoi(optarg) >= 0 && std::atoi(optarg) < 7){
					sort_method = std::atoi(optarg);
					std::cout << "Sorting method set to " << sort_method_names[sort_method] << "." << std::endl;
				}
				else
					std::cerr << "Invalid sorting method. Defaulting to " << sort_method_names[sort_method] << "." << std::endl;
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
					std::cerr << "Invalid screen dimensions. Defaulting to " << screen_width << "x" << screen_height << std::endl;
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
	window = SDL_CreateWindow("Sorting Visualizer",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		screen_width, screen_height,
		0);
	if (window == nullptr){
		log_SDL_error("CreateWindow");
		return 1;
	}

	// Create renderer
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == nullptr){
		log_SDL_error("CreateRenderer");
		return 1;
	}

	// Initialize SDL's True Type Fonts functionality
	if (TTF_Init() < 0) {
 		std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
		SDL_DestroyWindow(window);
		return 1;
	}

	// Load the TTF font
	load_font();
	if (g_font == NULL){
		std::cerr << "Font load error." << std::endl;
		return 1;
	}

	// Create and shuffle (num_elems) elements
	std::vector<uint16_t>(num_elems).swap(elems);
	std::iota(std::begin(elems), std::end(elems), 1);
	std::shuffle(elems.begin(), elems.end(), generator);

	// Create array for the bar Rects
	SDL_Rect arr[num_elems];
	bars = arr;

	// Sort using the specified/default sort method
	switch(sort_method){
		case 0:
			bubble_sort();
			break;
		case 1:
			selection_sort();
			break;
		case 2:
			insertion_sort();
			break;
		case 3:
			quicksort(0, num_elems - 1);
			break;
		case 4:
			bottom_up_mergesort();
			break;
		case 5:
			heapsort(0, num_elems);
			break;
		case 6:
			int depth = std::floor(log(num_elems)) * 2;
			introsort(depth, 0, num_elems - 1);
			break;
	}

	// Show sorted list
	create_frame(elems);

	if (check_sorted(elems))
		std::cout << "\nSorted!\n\n";
	else
		std::cerr << "\nError: Sorting Failure!\n\n";
		
	std::cout << "Total comparisons: " << num_comps << std::endl;
	std::cout << "Total swaps: " << num_swaps << std::endl;

	// Pause after sorting is complete
	SDL_Delay(2000);
	return 0;
}