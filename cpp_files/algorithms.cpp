#include "../headers/algorithms.h"

/**
* Checks if a vector contains a sorted sequence of elements.
*/
bool check_sorted(std::vector<uint16_t>& elems){
	uint16_t last_elem = elems[0];
	for (uint16_t i = 1; i < num_elems; i++){
		if (elems[i] < last_elem)
			return 0;
		last_elem = elems[i];
	}
	return 1;
}

/**
* A spectacularly inefficient sorting method, averages O((n+1)!) time.
*/
void bogo_sort(){
	while (!check_sorted(elems)){
		std::shuffle(elems.begin(), elems.end(), generator);
		create_frame(elems);
	}
}

/**
* Bubble sort, averages O(n^2) time.
*/
void bubble_sort(){
	uint16_t i, j;
	bool sorted = false;
	for (i = 0; i < num_elems - 1 && !sorted; i++){
		sorted = true;
		for (j = 0; j < num_elems - i - 1; j++){
			if (elems[j] > elems[j + 1]){
				std::swap(elems[j], elems[j + 1]);
				num_swaps++;
				sorted = false;
			}
			num_comps++;
			
			// Visualization code below.
			elems_accessed.push_back(j);
			elems_accessed.push_back(j + 1);
			create_frame(elems);

			check_exit();
		}
	}
}

/**
* Selection sort, averages O(n^2) time.
*/
void selection_sort(){
	uint16_t i, j, min;
	uint16_t n = elems.size();

	for (i = 0; i < n - 1; i++){
		min = i;
		for (j = i + 1; j < n; j++){
			if (elems[j] < elems[min])
				min = j;
			num_comps++;

			// Visualization code below.
			elems_accessed.push_back(min);
			elems_accessed.push_back(j);
			create_frame(elems);

			check_exit();
		}
		if (min != i){
			std::swap(elems[min], elems[i]);
			num_swaps++;

			// Visualization code below.
			elems_accessed.push_back(min);
			elems_accessed.push_back(i);
			create_frame(elems);
		}
	}
}

/**
* Insertion sort, averages O(n^2) time.
*/
void insertion_sort(){
	uint16_t i, j;
	for (i = 1; i < elems.size(); i++){
		for (j = i; j > 0 && elems[j - 1] > elems[j]; j--){
			num_comps++;
			std::swap(elems[j], elems[j - 1]);
			num_swaps++;

			// Visualization code below.
			elems_accessed.push_back(j);
			elems_accessed.push_back(j - 1);
			create_frame(elems);

			check_exit();
		}
	}
}

/**
* Quicksort, averages O(n*log(n)) time.
*/
void quicksort(uint16_t start_idx, uint16_t end_idx){
	if (start_idx < end_idx){
		uint16_t pivot = quicksort_partition(start_idx, end_idx);
		if (pivot != 0)
			quicksort(start_idx, pivot - 1);
		quicksort(pivot + 1, end_idx);
	}
}

/**
* Partition function of quicksort.
*/
uint16_t quicksort_partition(uint16_t start_idx, uint16_t end_idx){
	// Set the pivot to the median of the first, middle, and last elements.
	uint16_t mid_idx = (start_idx + end_idx) / 2;
	if (elems[mid_idx] < elems[start_idx]){
		std::swap(elems[start_idx], elems[mid_idx]);
		num_swaps++;
	}
	if (elems[end_idx] < elems[start_idx]){
		std::swap(elems[start_idx], elems[end_idx]);
		num_swaps++;
	}
	if (elems[mid_idx] < elems[end_idx]){
		std::swap(elems[mid_idx], elems[end_idx]);
		num_swaps++;
	}
	uint16_t pivot = elems[end_idx];
	num_comps += 3;

	uint16_t i = start_idx;
	for (uint16_t j = start_idx; j < end_idx; j++){
		num_comps++;
		if (elems[j] < pivot){
			elems_accessed.push_back(i);
			if (i != j){
				std::swap(elems[j], elems[i]);
				num_swaps++;
			}
			i++;
		}

		elems_accessed.push_back(end_idx);
		elems_accessed.push_back(j);
		create_frame(elems);
		
		check_exit();
	}
	if (i < end_idx)
		std::swap(elems[i], elems[end_idx]);
	
	elems_accessed.push_back(i);
	elems_accessed.push_back(end_idx);
	create_frame(elems);
	
	return i;
}

/**
* Mergesort, averages O(n*log(n)) time.
*/
void bottom_up_mergesort(){
	// Create copy of elems
	std::vector<uint16_t> B(elems);

	int32_t width;
	uint16_t i, right_idx, end_idx;
	for (width = 1; width < num_elems; width = 2 * width){
		for (i = 0; i < num_elems; i = i + 2 * width){
			right_idx = std::min(i+width, (int) num_elems);
			end_idx = std::min(i+2*width, (int) num_elems);
			bottom_up_merge(i, right_idx, end_idx, B);
		}
		// Copy B into elems
		elems = B;
	}
}

/**
* Merge function of mergesort.
*/
void bottom_up_merge(uint16_t left_idx, uint16_t right_idx, uint16_t end_idx, std::vector<uint16_t>& B){
	uint16_t i = left_idx;
	uint16_t j = right_idx;
	uint16_t k;
	for (k = left_idx; k < end_idx; k++){
		elems_accessed.push_back(i);
		elems_accessed.push_back(j);

		num_comps++;
		if (i < right_idx && (j >= end_idx || elems[i] <= elems[j])){
			B[k] = elems[i];
			i++;
		} else {
			B[k] = elems[j];
			j++;
		}
		num_swaps++;

		elems_accessed.push_back(k);
		create_frame(B);

		check_exit();
	}
}

/**
* Heapsort, averages O(n*log(n)) time.
*/
void heapsort(uint16_t start_idx, uint16_t end_idx){
	heapify(start_idx, end_idx);
	uint16_t end = end_idx - 1;
	while (end > start_idx){
		std::swap(elems[end], elems[start_idx]);
		num_swaps++;

		elems_accessed.push_back(start_idx);
		elems_accessed.push_back(end);
		create_frame(elems);

		end--;
		sift_down(start_idx, start_idx, end);
		
		check_exit();
	}
}

/**
* Initial heap creation for heapsort.
*/
void heapify(uint16_t start_idx, uint16_t end_idx){
	int32_t sift_idx;
	for (sift_idx = heap_parent(start_idx, end_idx - 1); sift_idx >= start_idx; sift_idx--){
		sift_down(start_idx, sift_idx, end_idx - 1);
		check_exit();
	}
}

/**
* Sifting function of heapsort.
*/
void sift_down(uint16_t start_idx, uint16_t sift_idx, uint16_t end_idx){
	uint16_t child, swap;
	uint16_t root = sift_idx;
	while (heap_left_child(start_idx, root) <= end_idx){
		child = heap_left_child(start_idx, root);
		swap = root;
		elems_accessed.push_back(root);

		num_comps++;
		if (elems[swap] < elems[child])
			swap = child;
		num_comps++;
		if (child+1 <= end_idx && elems[swap] < elems[child+1])
			swap = child + 1;
		if (swap == root)
			return;
		else{
			std::swap(elems[root], elems[swap]);
			num_swaps++;
			root = swap;
			elems_accessed.push_back(root);
		}

		elems_accessed.push_back(child);
		if (child+1 <= end_idx)
			elems_accessed.push_back(child+1);
		create_frame(elems);
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
void introsort(uint16_t max_depth, uint16_t start_idx, uint16_t end_idx){
	uint16_t pivot = quicksort_partition(start_idx, end_idx);
	if (end_idx - start_idx <= 1)
		return;
	else if (max_depth == 0)
		heapsort(start_idx, end_idx + 1);
	else{
		introsort(max_depth - 1, start_idx, pivot);
		introsort(max_depth - 1, pivot + 1, end_idx);
	}
}

/**
* Shellsort, time complexity depends on gap sequence.
*/
void shellsort(){
	// Create gap sequence (simplified Tokuda)
	std::vector<uint16_t> gaps;
	uint32_t last_gap = 1;
	while (last_gap < num_elems / 2){
		gaps.push_back(last_gap);
		last_gap = std::ceil(last_gap * 2.25 + 1);
	}
	std::reverse(gaps.begin(), gaps.end());

	uint16_t i, j, k, temp, gap;
	for (i = 0; i < gaps.size(); i++){
		gap = gaps[i];
		for (j = gap; j < num_elems; j++){
			elems_accessed.push_back(j);
			temp = elems[j];
			num_comps++;
			for (k = j; k >= gap && elems[k - gap] > temp; k -= gap){
				num_comps++;
				elems_accessed.push_back(k);
				elems_accessed.push_back(k - gap);
				elems[k] = elems[k - gap];
				num_swaps++;
			}
			elems_accessed.push_back(k);
			elems[k] = temp;
			num_swaps++;

			create_frame(elems);

			check_exit();
		}
	}
}

/**
* Check for requested shutdown.
*/
void check_exit(){
	while (SDL_PollEvent(&event))
		if (event.type == SDL_QUIT){
			std::cout << "\nExiting.\n";
			exit(0);
		}
}