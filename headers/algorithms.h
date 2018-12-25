#ifndef __ALGS_H__
#define __ALGS_H__

#include "../globals.h"
#include "visuals.h"

bool check_sorted(std::vector<uint16_t>& elems);

void bogo_sort();

void bubble_sort();

void selection_sort();

void insertion_sort();

void quicksort(uint16_t start_idx, uint16_t end_idx);

uint16_t quicksort_partition(uint16_t start_idx, uint16_t end_idx);

void bottom_up_mergesort();

void bottom_up_merge(uint16_t left_idx, uint16_t right_idx, uint16_t end_idx, std::vector<uint16_t>& B);

void heapsort(uint16_t start_idx, uint16_t end_idx);

void heapify(uint16_t start_idx, uint16_t end_idx);

void sift_down(uint16_t start_idx, uint16_t start, uint16_t end);

uint16_t heap_parent(uint16_t start_idx, uint16_t i);

uint16_t heap_left_child(uint16_t start_idx, uint16_t i);

uint16_t heap_right_child(uint16_t start_idx, uint16_t i);

void introsort(uint16_t max_depth, uint16_t start_idx, uint16_t end_idx);

void shellsort();

void check_exit();

#endif