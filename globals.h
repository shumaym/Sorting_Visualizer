#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <random>

extern uint16_t screen_width;
extern uint16_t screen_height;
extern const float SCREEN_MARGINS;
extern const float BAR_SEPARATION;

extern const uint8_t COLOUR_BACKGROUND[3];
extern const uint8_t COLOUR_BARS_WHITE[3];
extern const uint8_t COLOUR_BARS_RED[3];

extern uint16_t frame_delay_ms;

extern std::mt19937 generator;

#endif