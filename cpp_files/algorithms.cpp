#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>

#include "../headers/algorithms.h"

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
		create_frame(elems, num_elems, renderer, bars, elems_accessed);
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
			create_frame(elems, num_elems, renderer, bars, elems_accessed);
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
			create_frame(elems, num_elems, renderer, bars, elems_accessed);
		}
		if (min != i){
			std::swap(elems[min], elems[i]);

			// Visualization code below.
			elems_accessed.push_back(min);
			elems_accessed.push_back(i);
			create_frame(elems, num_elems, renderer, bars, elems_accessed);
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
			create_frame(elems, num_elems, renderer, bars, elems_accessed);
		}
	}
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
		int pivot = quicksort_partition(elems, start_idx, end_idx, num_elems, bars, renderer);
		quicksort(elems, num_elems, start_idx, pivot - 1, bars, renderer);
		quicksort(elems, num_elems, pivot + 1, end_idx, bars, renderer);
	}
}

/**
* Partition function of quicksort.
*/
uint16_t quicksort_partition(
		std::vector<uint16_t>& elems,
		uint16_t start_idx,
		uint16_t end_idx,
		uint16_t num_elems,
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
		create_frame(elems, num_elems, renderer, bars, elems_accessed);
	}
	if (i < end_idx)
		std::swap(elems[i], elems[end_idx]);
	
	// Visualization code below.
	elems_accessed.push_back(i);
	elems_accessed.push_back(end_idx);
	create_frame(elems, num_elems, renderer, bars, elems_accessed);
	
	return i;
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
		// Copy B into A.
		A = B;
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
		create_frame(B, num_elems, renderer, bars, elems_accessed);
	}
}

/**
* Heapsort, averages O(n*log(n)) time.
*/
void heapsort(
		std::vector<uint16_t>& A,
		int start_idx,
		int end_idx,
		uint16_t num_elems,
		SDL_Rect bars[],
		SDL_Renderer *renderer){

	std::vector<uint16_t> elems_accessed;
	heapify(A, start_idx, end_idx, num_elems, bars, renderer);

	int end = end_idx;
	while (end > start_idx){
		std::swap(A[end], A[start_idx]);

		// Visualization code below.
		elems_accessed.push_back(start_idx);
		elems_accessed.push_back(end);
		create_frame(A, num_elems, renderer, bars, elems_accessed);

		end--;
		sift_down(A, start_idx, start_idx, end, num_elems, bars, renderer);
	}
}

/**
* Initial heap creation for heapsort.
*/
void heapify(
		std::vector<uint16_t>& A,
		int start_idx,
		int end_idx,
		uint16_t num_elems,
		SDL_Rect bars[],
		SDL_Renderer *renderer){

	for (int sift_idx = heap_parent(start_idx, end_idx); sift_idx >= start_idx; sift_idx--)
		sift_down(A, start_idx, sift_idx, end_idx, num_elems, bars, renderer);
}

/**
* Sifting function of heapsort.
*/
void sift_down(
		std::vector<uint16_t>& A,
		int start_idx,
		int sift_idx,
		int end_idx,
		uint16_t num_elems,
		SDL_Rect bars[],
		SDL_Renderer *renderer){

	std::vector<uint16_t> elems_accessed;
	int root = sift_idx;
	int child, swap;
	while (heap_left_child(start_idx, root) <= end_idx){
		child = heap_left_child(start_idx, root);
		swap = root;

		if (A[swap] < A[child])
			swap = child;
		if (child+1 <= end_idx && A[swap] < A[child+1])
			swap = child + 1;
		if (swap == root)
			return;
		else{
			std::swap(A[root], A[swap]);
			root = swap;
		}

		// Visualization code below.
		elems_accessed.push_back(root);
		elems_accessed.push_back(child);
		if (child+1 <= end_idx)
			elems_accessed.push_back(child+1);
		create_frame(A, num_elems, renderer, bars, elems_accessed);
	}
}

/**
* Returns the index of the parent of the given heap node.
*/
uint16_t heap_parent(uint16_t start_idx, uint16_t i){
	return start_idx + std::floor((i - start_idx - 1) / 2);
}

/**
* Returns the index of the left child of the given heap node.
*/
uint16_t heap_left_child(uint16_t start_idx, uint16_t i){
	return start_idx + 2*(i - start_idx) + 1;
}

/**
* Returns the index of the right child of the given heap node.
*/
uint16_t heap_right_child(uint16_t start_idx, uint16_t i){
	return start_idx + 2*(i - start_idx) + 2;
}

/**
* Introsort, averages O(n*log(n)) time.
* Uses quicksort until a bad case is encountered,
* then switches to heapsort for the current subsection.
*/
void introsort(
		std::vector<uint16_t>& A,
		int max_depth,
		int start_idx,
		int end_idx,
		uint16_t num_elems,
		SDL_Rect bars[],
		SDL_Renderer *renderer){

	std::vector<uint16_t> elems_accessed;
	int pivot = quicksort_partition(A, start_idx, end_idx, num_elems, bars, renderer);
	if (end_idx - start_idx <= 1)
		return;
	else if (max_depth == 0){
		heapsort(A, start_idx, end_idx, num_elems, bars, renderer);
	}
	else{
		introsort(A, max_depth - 1, start_idx, pivot, num_elems, bars, renderer);
		introsort(A, max_depth - 1, pivot + 1, end_idx, num_elems, bars, renderer);
	}
}