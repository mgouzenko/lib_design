#include <cstring> 
#include <cstdlib> 
#include <algorithm>
#include <iterator> 
#include "custom_random.hpp"
#include <string> 
#include <climits>
#include <chrono> 
#include <functional> 
#include <iostream>
#include <fstream>

const static int MAX_LENGTH = 10; 
const static int MIN_LENGTH = 8; 

using namespace std;

template<typename Function> 
int time_function(Function f){
	using std::chrono::system_clock; 
	using std::chrono::microseconds;
	
	auto start = system_clock::now();
	f(); 
	auto stop = system_clock::now(); 

	return std::chrono::duration_cast<microseconds>(stop - start).count();
} 

template<typename Type>
int less_than_comparison(Type obj1, Type obj2){
	return obj1 < obj2; 
}

template<typename Type> 
int less_than_comparison_void(const void* obj1, const void* obj2){
	return less_than_comparison( *(reinterpret_cast<const Type*> (obj1)), *(reinterpret_cast<const Type*>(obj2)) );
}

int string_comparison(const char* str1, const char* str2){
	return strcmp(str1, str2)<0?1:0;
}

int string_comparison_void(const void* obj1, const void* obj2){
	return string_comparison( reinterpret_cast<const char*> (obj1), reinterpret_cast<const char*>(obj2));
}

int no_case_string_comparison(const char* str1, const char* str2){
	return strcasecmp(str1, str2)<0?1:0;
}

int no_case_string_comparison_void(const void* obj1, const void* obj2){
	return no_case_string_comparison( reinterpret_cast<const char*> (obj1), reinterpret_cast<const char*>(obj2));
}

template<typename Comparator, typename Iter>
int time_generalized_sort(Iter begin_itr, Iter end_itr, Comparator c){ 		
	auto sort_function = bind(sort<Iter, Comparator>, begin_itr, end_itr, c); 	
	return time_function(sort_function); 
}

template<typename Comparator, typename Iter>
int time_list_sort(Iter begin_itr, Iter end_itr, int size, Comparator c){
	int total_time = 0; 

	using std::chrono::system_clock; 
	using std::chrono::microseconds;

	auto start = system_clock::now(); 
	vector<string> vec(size, ""); 
	for(int i=0; begin_itr!=end_itr; i++) vec[i] = *begin_itr++; 
	auto stop = system_clock::now(); 

	total_time += std::chrono::duration_cast<microseconds>(stop - start).count(); 			
	total_time += time_generalized_sort(vec.begin(), vec.end(), c); 

	return total_time; 
}


template<typename Type, typename Pointer> 
int time_generalized_qsort(Pointer cont, int num, int size, int (*func)(Type, Type) ){
	auto bound_func = bind(qsort, static_cast<void*> (&(*cont)), num, size, func);  
	return time_function(bound_func);  
}

template<typename Type, typename Iter>
int time_list_qsort(Iter begin_itr, Iter end_itr, int num, int size, int (*func)(Type, Type)){
	int total_time = 0; 

	using std::chrono::system_clock; 
	using std::chrono::microseconds;

	auto start = system_clock::now(); 
	vector<string> vec(num, ""); 
	for(int i=0; begin_itr!=end_itr; i++) vec[i] = *begin_itr++; 
	auto stop = system_clock::now(); 

	total_time += std::chrono::duration_cast<microseconds>(stop - start).count(); 			
	total_time += time_generalized_qsort(vec.begin(), num, size, func); 

	return total_time;
}


string rand_string(random_number_generator& rand_gen, int min = MIN_LENGTH, int max = MAX_LENGTH){
	string rand_str; 
	rand_gen.set_min(min); 
	rand_gen.set_max(max); 
	int length = rand_gen(); 

	rand_gen.set_min(97); 
	rand_gen.set_max(122);

	for(int i=0 ; i<length; i++){
		int c = rand_gen(); 
		int next_char = rand_gen(); 
		
		if(c <= (97+122)/2){
			next_char -= 32; 
		} 

		rand_str += to_string(next_char); 
	}	

	return rand_str; 
}

char* rand_char_array(random_number_generator& rand_gen, int min=MIN_LENGTH, int max = MAX_LENGTH){

	string word = rand_string(rand_gen, min, max);
	char* cstring =  const_cast<char*> (word.c_str()); 
	
	char* new_word = new char[strlen(cstring)+1]; 
	strcpy(new_word, cstring); 

	//cout << new_word;
	return new_word; 
}

int rand_int(random_number_generator& rand_gen){
	rand_gen.set_max(INT_MAX); 
	rand_gen.set_min(INT_MIN); 

	return rand_gen(); 
} 

template<typename Iter, typename Generator> 
void fill_random(random_number_generator rand_gen, Iter begin_itr, Iter end_itr, Generator gen){
	while(begin_itr!=end_itr){
		auto item = gen(rand_gen); 
		*begin_itr++ = item; 
	} 

}

template<typename Iter> 
void print_pairs(Iter begin, Iter end, ofstream& file){
	//Advance the beginning iterator, printing first the number of elements, and then the time. 
	while(begin!=end){
		file << (*begin).first << "\t" << (*begin).second << endl; 
		++begin; 
	}
	cout << endl;
}

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
	time = time_generalized_sort(begin(new_string_vec), end(new_string_vec), &less_than_comparison<string>);
	sort_results.push_back(pair<string, int>("vector<string>", time));

	time = time_generalized_qsort(begin(string_vec), num, sizeof(string), &less_than_comparison_void<string>);
	qsort_results.push_back(pair<string, int>("vector<string>", time));  

	/********************************** Test for vector<char*> ******************************/
	rand_gen.seed(s); 
	vector<char*> char_pointer_vec(num, 0); 
	fill_random(rand_gen, begin(char_pointer_vec), end(char_pointer_vec), char_array_fn);   
	
	vector<char*> new_char_pointer_vec = char_pointer_vec; 
	time = time_generalized_sort(new_char_pointer_vec.begin(), new_char_pointer_vec.end(), &string_comparison); 
	sort_results.push_back(pair<string, int>("vector<char*>", time));

	new_char_pointer_vec = char_pointer_vec; 
	time = time_generalized_qsort(begin(new_char_pointer_vec), num, sizeof(char*), &string_comparison_void); 
	qsort_results.push_back(pair<string, int>("vector<char*>", time));  

	/********************************* Test for vector<char*> - case insensitive *************/	
	new_char_pointer_vec = char_pointer_vec; 
	time = time_generalized_sort(begin(new_char_pointer_vec), end(new_char_pointer_vec), &no_case_string_comparison); 
	sort_results.push_back(pair<string, int>("vector<char*> - case insensitive", time));

	new_char_pointer_vec = char_pointer_vec; 
	time = time_generalized_qsort(begin(new_char_pointer_vec), num, sizeof(char*), &no_case_string_comparison_void); 
	qsort_results.push_back(pair<string, int>("vector<char*> - case insensitive", time)); 

	for(auto c: char_pointer_vec) delete[] c; 


	/********************************** Test for list<string> ********************************/
	rand_gen.seed(s); 
	list<string> string_list(num, "");  
	fill_random(rand_gen, begin(string_list), end(string_list), rand_string_fn); 

	list<string> new_string_list = string_list; 
	time = time_list_sort(begin(new_string_list), end(new_string_list), num, &less_than_comparison<string>);  
	sort_results.push_back(pair<string, int>("list<string>", time));

	time = time_list_qsort(begin(new_string_list), end(new_string_list), num, sizeof(string), &less_than_comparison_void<string>); 
	qsort_results.push_back(pair<string, int>("list<string>", time));  

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
