CXXFLAGS = -std=c++11 -O2 

main: main.cpp

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
