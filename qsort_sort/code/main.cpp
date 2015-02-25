#include <list>
#include <vector> 
#include "test.hpp"

using namespace std; 

void run_tests(int num, int s){	
	auto char_array_fn =
		[](random_number_generator& rand){return rand_char_array(rand);}; 
	auto uppercase_char_array_fn = 
		[](random_number_generator& rand){return rand_char_array(rand, true);};  
	auto rand_string_fn =
		[](random_number_generator& rand){return rand_string(rand);};

	random_number_generator rand_gen; 
	int time; 
	vector<pair<string, int>> qsort_results; 
	vector<pair<string, int>> sort_results; 

	/********************************** Test for int array[] ****************************/ 
	rand_gen.seed(s); 
	
	int arr[num]; 
	fill_random(rand_gen, arr, arr+num, rand_int); 	
	int new_arr[num]; 
	memcpy(new_arr, arr, num * sizeof(int)); 

	time = time_generalized_sort(new_arr, new_arr+num, &less_than_comparison<int>);
	sort_results.push_back(pair<string, int>("int[]", time));  

	time = time_generalized_qsort(arr, num, sizeof(int), &less_than_comparison_void<int>); 
	qsort_results.push_back(pair<string, int>("int[]", time));

	/********************************** Test for vector<string> ***************************/
	rand_gen.seed(s); 
	vector<string> string_vec(num, "");
	fill_random(rand_gen, begin(string_vec), end(string_vec), rand_string_fn); 	
	vector<string> new_string_vec = string_vec; 

	time = time_generalized_qsort(&string_vec[0], num, sizeof(string), &less_than_comparison_void<string>);
	qsort_results.push_back(pair<string, int>("vector<string>", time));  
	
	time = time_generalized_sort(begin(new_string_vec), end(new_string_vec), [](string s1, string s2) 
																						{
																							return s1 < s2; 
																						}
																						);
	sort_results.push_back(pair<string, int>("vector<string>", time));

	
	/********************************** Test for vector<char*> ******************************/
	rand_gen.seed(s); 
	vector<char*> char_pointer_vec(num, 0); 
	fill_random(rand_gen, begin(char_pointer_vec), end(char_pointer_vec), char_array_fn);   
	
	vector<char*> new_char_pointer_vec = char_pointer_vec; 
	time = time_generalized_sort(new_char_pointer_vec.begin(), new_char_pointer_vec.end(), &string_comparison); 
	sort_results.push_back(pair<string, int>("vector<char*>", time));

	new_char_pointer_vec = char_pointer_vec; 
	time = time_generalized_qsort(&new_char_pointer_vec[0], num, sizeof(char*), &string_comparison_void); 
	qsort_results.push_back(pair<string, int>("vector<char*>", time));  

	/********************************* Test for vector<char*> - case insensitive *************/	
	new_char_pointer_vec = char_pointer_vec; 
	time = time_generalized_sort(begin(new_char_pointer_vec), end(new_char_pointer_vec), &no_case_string_comparison); 
	sort_results.push_back(pair<string, int>("vector<char*> - case insensitive", time));

	new_char_pointer_vec = char_pointer_vec;
	time = time_generalized_qsort(&new_char_pointer_vec[0], num, sizeof(char*), &no_case_string_comparison_void); 
	qsort_results.push_back(pair<string, int>("vector<char*> - case insensitive", time)); 

	for(auto c: char_pointer_vec) delete[] c; 


	/********************************** Test for list<string> ********************************/
	rand_gen.seed(s); 
	list<string> string_list(num, "");  
	fill_random(rand_gen, begin(string_list), end(string_list), rand_string_fn); 
	list<string> new_string_list = string_list; 
	
	time = time_list_qsort(begin(string_list), end(string_list), num, sizeof(string), &less_than_comparison_void<string>); 
	qsort_results.push_back(pair<string, int>("list<string>", time));  	

	
	time = time_list_sort(begin(new_string_list), end(new_string_list), num, &less_than_comparison<string>);  
	sort_results.push_back(pair<string, int>("list<string>", time));
	
	

	/******************************* Print Results *******************************************/
	string seed_str = to_string(s); 
	string name1 = "seed_"+seed_str+"_sort.txt";  
    string name2 = "seed_"+seed_str+"_qsort.txt"; 

	ofstream file;
	file.open(name1); 
	print_pairs(sort_results.begin(), sort_results.end(), file);

	ofstream file2; 
	file2.open(name2);
	print_pairs(qsort_results.begin(), qsort_results.end(), file2); 
}


int main(int argc, char** argv){
	if(argc!=3){
		cout << "Usage: main <num_elements> <seed>\n"; 
		return 1; 
	}
	
	run_tests(stoi(argv[1]), stoi(argv[2])); 
	return 0; 
}
