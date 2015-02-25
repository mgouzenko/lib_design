#include <list>
#include <vector> 
#include "test.hpp"

using namespace std; 

void run_tests(int num, int s){	
	auto rand_string_fn =
		[](random_number_generator& rand){return rand_string(rand);};

	random_number_generator rand_gen; 
	rand_gen.seed(s); 

	vector<string> string_vec(num, "");
	fill_random(rand_gen, begin(string_vec), end(string_vec), rand_string_fn); 	

	time_generalized_qsort(&string_vec[0], num, sizeof(string), &less_than_comparison_void<string>);	
}


int main(int argc, char** argv){
	if(argc!=3){
		cout << "Usage: main <num_elements> <seed>\n"; 
		return 1; 
	}
	
	run_tests(stoi(argv[1]), stoi(argv[2])); 
	return 0; 
}
