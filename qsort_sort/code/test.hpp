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

const static int MAX_LENGTH = 50; 
const static int MIN_LENGTH = 8; 

using namespace std;

//Prints what's between the iterators. Debugging only. 
template<typename Iter> 
void print_list(Iter b, Iter e){
	while(b!=e) cout << *b++ << " " ;
	cout << endl; 
} 

//Times Function f in microseconds. 
template<typename Function> 
int time_function(Function f){
	using std::chrono::system_clock; 
	using std::chrono::microseconds;	
	auto start = system_clock::now();
	f(); 
	auto stop = system_clock::now(); 
	return std::chrono::duration_cast<microseconds>(stop - start).count();
} 

//Less than comparison for use with sort (int[] and vector<string>);
template<typename Type>
inline int less_than_comparison(Type obj1, Type obj2){
	return obj1 < obj2; 
}

//Less-than comparison for use with qsort. Note that is NOT a strict-weak
//ordering. Void pointers are cast into Type, dererfenced, and compared. 
template<typename Type> 
int less_than_comparison_void(const void* obj1, const void* obj2){
	const Type* type_obj1 = reinterpret_cast<const Type*>(obj1);
	const Type* type_obj2 = reinterpret_cast<const Type*>(obj2);
	
	if(*type_obj1 < *type_obj2 ) return -1; 
	if(*type_obj1 == *type_obj2) return 0; 
	else return 1; 
}

//Simple c-style string comparison for use with sort. Note the strict-weak ordering 
inline int string_comparison(const char* str1, const char* str2){
	return strcmp(str1, str2)<0?1:0;
}

//String comparison for use with qsort. Note the lack of strict-weak ordering. 
int string_comparison_void(const void* obj1, const void* obj2){
	// char_pointer is a pointer to a const pointer to a char. 
	auto char_pointer1 = reinterpret_cast<char* const *>(obj1);
	auto char_pointer2 = reinterpret_cast<char* const *>(obj2);
	return strcmp(*char_pointer1, *char_pointer2); 
}

//Simple c-style case-insensitive string comparison for use with sort.
//Note that strcasecmp is defined as Posix specification.
inline int no_case_string_comparison(const char* str1, const char* str2){
	return strcasecmp(str1, str2)<0?1:0;
}

//Case-insensitive string comparison for use with qsort. 
int no_case_string_comparison_void(const void* obj1, const void* obj2){
	auto char_pointer1 = reinterpret_cast<char *const *>(obj1);
	auto char_pointer2 = reinterpret_cast<char *const *>(obj2);
	return strcasecmp(*char_pointer1, *char_pointer2); 
}

//Binds the arguments to sort properly and times the execution of the 
//resulting function object. 
template<typename Comparator, typename Iter>
int time_generalized_sort(Iter begin_itr, Iter end_itr, Comparator c){ 		
	auto sort_function = bind(sort<Iter, Comparator>, begin_itr, end_itr, c); 	
	return time_function(sort_function); 
}

//Fills container pointed to by dest with whatever is between src-begin(inclusive) and src_end (not inclusive) 
template<typename DestIter, typename SrcIter>
void container_conversion(DestIter dest, SrcIter src_begin, SrcIter src_end){
	while (src_begin!=src_end) *dest++ = *src_begin++; 
}

//Times the sorting of linked list with std::sort. Linked list is converted into a vector, which is sorted, and then
//converted back. 
template<typename Comparator, typename Iter>
int time_list_sort(Iter begin_itr, Iter end_itr, int size, Comparator c){
	int total_time = 0; 

	//Fill the new vector with whatever is stored at *begin_itr. This is arbitrary but ensures vec's size is correct.
	vector<string> vec(size, *begin_itr); 
	
	//This function object moves whatever is in the list to the vector. 
	auto list_2_vec = bind( container_conversion<decltype(vec.begin()), Iter>, vec.begin(), begin_itr, end_itr);  
	
	//This function object moves the contents of the vector back into the list. 
	auto vec_2_list = bind( container_conversion<Iter, decltype(vec.begin())>, begin_itr, vec.begin(), vec.end()); 

	//Time the conversion of list to vector, then the sorting of vector, then the conversion of vector to list. 
	total_time += time_function(list_2_vec);
	total_time += time_generalized_sort(vec.begin(), vec.end(), c); 
	total_time += time_function(vec_2_list); 

	return total_time; 
}

//Similar idea to sort - just bind the function args of qsort and execute. 
template<typename Type, typename Pointer> 
int time_generalized_qsort(Pointer cont, int num, int size, int (*func)(Type, Type) ){
	auto bound_func = bind(qsort, static_cast<void*> (cont), num, size, func);  
	return time_function(bound_func);  
} 

//Similar idea to time_list_sort. 
template<typename Iter, typename Type>
int time_list_qsort(Iter begin_itr, Iter end_itr, int num, int size, int (*func)(Type, Type)){
	int total_time = 0; 

	vector<string> vec(num, *begin_itr); 
	auto list_2_vec = bind( container_conversion<decltype(vec.begin()), Iter>, vec.begin(), begin_itr, end_itr);  
	auto vec_2_list = bind( container_conversion<Iter, decltype(vec.begin())>, begin_itr, vec.begin(), vec.end()); 

	total_time += time_function(list_2_vec);  
	total_time += time_generalized_qsort(&vec[0], num, size, func); 
	total_time += time_function(vec_2_list); 

	return total_time;
}

//Generates a random c++ style string.
string rand_string(random_number_generator& rand_gen, int min = MIN_LENGTH, int max = MAX_LENGTH){
	string rand_str; 
	
	//Get length of the string. 
	rand_gen.set_min(min); 
	rand_gen.set_max(max); 
	int length = rand_gen(); 

	//Ensure that we have ints corresponding to a-z
	rand_gen.set_min(97); 
	rand_gen.set_max(122);

	for(int i=0 ; i<length; i++){
		//c will determine the case of the character
		int c = rand_gen(); 
		int next_char = rand_gen(); 
		
		//If c is in the lower half of [97-122], capitalize. 
		if(c <= (97+122)/2) next_char -= 32; 
		rand_str += static_cast<char>(next_char); 
	}

	return rand_str; 
}

//Returns a heap-allocated char array. This is probably a pretty bad thing to do, but hey, we're 
//forced to use c-style semantics for qsort =(
char* rand_char_array(random_number_generator& rand_gen, int min=MIN_LENGTH, int max = MAX_LENGTH){
	string word = rand_string(rand_gen, min, max);
	char* cstring =  const_cast<char*> (word.c_str()); 
	char* new_word = new char[strlen(cstring)+1]; 
	strcpy(new_word, cstring); 
	return new_word; 
}

//Generate a totally random int. 
int rand_int(random_number_generator& rand_gen){
	rand_gen.set_max(INT_MAX); 
	rand_gen.set_min(INT_MIN); 
	return rand_gen(); 
} 

//Fill the area between begin_itr (inclusive) and end_itr (not inclusive) with whatever is spit out by gen. 
template<typename Iter, typename Generator> 
void fill_random(random_number_generator rand_gen, Iter begin_itr, Iter end_itr, Generator gen){
	while(begin_itr!=end_itr){
		auto item = gen(rand_gen); 
		*begin_itr++ = item; 
	} 
}

//Write results to file. 
template<typename Iter> 
void print_pairs(Iter begin, Iter end, ofstream& file){
	while(begin!=end){
		file << (*begin).first << "\t" << (*begin).second << endl; 
		++begin; 
	}
}
