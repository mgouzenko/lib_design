#include <iostream>
#include <cstdlib>
#include "test.hpp"

using namespace std;  
/* Main function starts tests with the appropriate element type, min, max, seed, step, and filenames. */

int main(int argc, char** argv){
	if(argc<7) {
		cout << "Usage: speed_test <vector outfile> <list outfile> <seed> <min> <max> <step>\n";
		return 1; 
	}
	
	string name1 = argv[1]; 
	string name2 = argv[2]; 
	string seed_string = argv[3]; 
	string min_string = argv[4];
	string max_string = argv[5]; 
	string step_string = argv[6];

	//Turn the necessary string arguments into integers. 
	int min = stoi(min_string); 
	int max = stoi(max_string); 
	int step = stoi(step_string); 
	int seed = stoi(seed_string);
	
	string title; 
	string outfile = "seed_" + seed_string + "_min_" +min_string + "_max_" + max_string + "_step_" + step_string; 
	if(argc == 8){
		string flag = argv[7];
		//If the --large-container flag is set, run the test with large_containers. 
		if(flag=="--large-container"){
			test::run_tests<test::large_container>(min, max, step, name1, name2, seed);
			title = "\"Vector VS List - Large Container, Seed: " + seed_string + "\"";
			outfile += "_lc.png"; 
		}

		else{
			cout << "Unrecognized flag: " << flag << endl;
			return 0; 
		} 
	}
	else{ 
		test::run_tests<int>(min, max, step, name1, name2, seed); 
		title = "\"Vector VS List - Seed: " + seed_string + "\""; 
		outfile += ".png"; 
	}
	
	//Plot graphs. 
	string command = "sh graph.sh " + name1 + " " + name2 + " " + outfile + " " + title;  
	system(command.c_str());
	return 0;
}
