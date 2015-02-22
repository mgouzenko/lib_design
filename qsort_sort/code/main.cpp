#include <list>
#include <vector> 
#include "test.hpp"

using namespace std; 

int main(int argc, char** argv){
	if(argc!=3){
		cout << "Usage: main <num_elements> <seed>\n"; 
		return 1; 
	}
	
	run_tests(stoi(argv[1]), stoi(argv[2])); 
	return 0; 
} 

