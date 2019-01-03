# Sorting Visualizer

This program provides a visual demonstration of various sorting algorithms.

## Dependencies
*	<b>g++</b>
*	<a href="https://www.libsdl.org/download-2.0.php"><b>SDL2</b></a>
*	<b>SDL2_ttf</b>
*	<b>SDL2_image</b>

## Usage
Build with ```make```, then run with ```./sorting [OPTIONS]```

Options:

*	```-n N```: Number of elements in the set (default: 50)
*	```-d N | --frame-delay N```: Delay in ms after the window is refreshed (default: 50)
*	```--dimensions XxY```: Screen dimensions (default: 1500x1000)
*	```-h | --help```: Display the help page
*	```-s N | --sorting N```: Sorting method to use (default: quicksort)
    - ```0```: Bubble sort
    - ```1```: Selection sort
    - ```2```: Insertion sort
    - ```3```: Quicksort
    - ```4```: Mergesort
    - ```5```: Heapsort
    - ```6```: Introsort
    - ```7```: Shellsort