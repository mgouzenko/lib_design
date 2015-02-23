#include <cstdlib> 
#include <algorithm>
#include <iterator> 
#include "custom_random.hpp"
#include <string> 
#include <climits> 

const static int MAX_LENGTH = 50; 
const static int MIN_LENGTH = 8; 

using namespace std; 
template<typename Iter>
void generalized_sort(Iter begin_itr, Iter end_itr){
	sort(begin_itr, end_itr);  
}

template<>
void generalized_sort<list<string>>(list<string>& l){
	l.sort(); 
}

template<typename Type>
int comparator(void* obj1, void* obj2){
	return (static_cast<Type> (obj1)) < (static_cast<Type> (obj2)); 
}

template<typename Type, typename Container> 
void generalized_qsort(Container& cont, int num){
	qsort( static_cast<void*> (&cont), num, sizeof(Type), &comparator<Type>);  
}

string rand_word(random_number_generator& rand_gen, int min = MIN_LENGTH, int max = MAX_LENGTH){
	string rand_str; 
	
	rand_gen.set_min(min); 
	rand_gen.set_max(max); 
	int length = rand_gen(); 
	
	rand_gen.set_min(1); 
	rand_gen.set_max(255);

	for(int i=0 ; i<length; i++){
		rand_str += to_string(rand()); 
	}

	return rand_str; 
}

char* rand_char_array(random_number_generator& rand_gen, int min=MIN_LENGTH, int max = MAX_LENGTH){
	string word = rand_word(rand_gen, min, max);
	return const_cast<char*> (word.c_str()); 
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

// template<> 
// void fill_random<int[]>(random_number_generator rand_gen, int arr[]){
// 	rand_gen.set_max(INT_MAX); 
// 	rand_gen.set_min(INT_MIN); 
//
// 	for(int i = 0; i < 0 ; i++){
// 		arr[i] = rand_gen(); 	
// 	}
// }
