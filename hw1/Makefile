CC = clang++
CXXFLAGS = -std=c++11 -stdlib=libc++ 

speed_test: speed_test.o
speed_test.o: speed_test.cpp

graph:
	graph -T png vector.txt --top-label "Vector VS Linked List" --y-label "Milliseconds" -w .7 --bitmap-size "1600x1200" > plot.png 
