#include <list>
#include <vector> 
#include "test.hpp"

using namespace std; 

void run_tests(int num, int s){	
	auto rand_string_fn =
		[](random_number_generator& rand){return rand_string(rand);};

	random_number_generator rand_gen; 
	rand_gen.seed(s); 

	int arr_num[num];
	fill_random(rand_gen, arr_num, arr_num + num, rand_int); 	

	time_generalized_sort(arr_num, arr_num +num , &less_than_comparison<int>);	
}


int main(int argc, char** argv){
	if(argc!=3){
		cout << "Usage: main <num_elements> <seed>\n"; 
		return 1; 
	}
	
	run_tests(stoi(argv[1]), stoi(argv[2])); 
	return 0; 
}
