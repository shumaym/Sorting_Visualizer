CXX = g++ -std=c++11
CXXFLAGS = -Wall -O3
LDLIBS = -lSDL2 -lSDL2_ttf -lSDL2_image

sorting: main.cpp cpp_files/*.cpp
	$(CXX) $(CXXFLAGS) -o $@ main.cpp cpp_files/*.cpp $(LDLIBS)
	
clean:
	rm -f sorting