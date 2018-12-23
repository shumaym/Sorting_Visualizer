#ifndef __ALGS_H__
#define __ALGS_H__

#include "../globals.h"
#include "visuals.h"

int check_sorted(std::vector<uint16_t>& elems, uint16_t num_elems);

void bogo_sort(
		std::vector<uint16_t>& elems,
		uint16_t num_elems,
		SDL_Rect bars[],
		SDL_Renderer *renderer);

void bubble_sort(
		std::vector<uint16_t>& elems,
		uint16_t num_elems,
		SDL_Rect bars[],
		SDL_Renderer *renderer);

void selection_sort(
		std::vector<uint16_t>& elems,
		uint16_t num_elems,
		SDL_Rect bars[],
		SDL_Renderer *renderer);

void insertion_sort(
		std::vector<uint16_t>& elems,
		uint16_t num_elems,
		SDL_Rect bars[],
		SDL_Renderer *renderer);

void quicksort(
		std::vector<uint16_t>& elems,
		uint16_t num_elems,
		int start_idx,
		int end_idx,
		SDL_Rect bars[],
		SDL_Renderer *renderer);

uint16_t quicksort_partition(
		std::vector<uint16_t>& elems,
		uint16_t num_elems,
		uint16_t start_idx,
		uint16_t end_idx,
		SDL_Rect bars[],
		SDL_Renderer *renderer);

void bottom_up_mergesort(
		std::vector<uint16_t>& A,
		uint16_t num_elems,
		SDL_Rect bars[],
		SDL_Renderer *renderer);

void bottom_up_merge(
		std::vector<uint16_t>& A,
		uint16_t left_idx,
		uint16_t right_idx,
		uint16_t end_idx,
		std::vector<uint16_t>& B,
		uint16_t num_elems,
		SDL_Rect bars[],
		SDL_Renderer *renderer);

void heapsort(
		std::vector<uint16_t>& A,
		uint16_t num_elems,
		SDL_Rect bars[],
		SDL_Renderer *renderer);

void heapify(
		std::vector<uint16_t>& A,
		uint16_t count,
		uint16_t num_elems,
		SDL_Rect bars[],
		SDL_Renderer *renderer);

void sift_down(
		std::vector<uint16_t>& A,
		uint16_t start,
		uint16_t end,
		uint16_t num_elems,
		SDL_Rect bars[],
		SDL_Renderer *renderer);

uint16_t heap_parent(uint16_t i);

uint16_t heap_left_child(uint16_t i);

uint16_t heap_right_child(uint16_t i);

#endif