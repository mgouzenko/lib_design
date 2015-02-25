CXX = /usr/local/bin/g++ 
CXXFLAGS = -std=c++11 -O2 

main: main.cpp

string_qsort: string_qsort.cpp

string_sort: string_sort.cpp

.PHONY: clean
clean:
	rm main

.PHONY: all
all: clean main

.PHONY: test
test: all
	./main
	

graph:
	sh ./graph.sh vector.txt list.txt
