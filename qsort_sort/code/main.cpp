#include <list>
#include <vector> 
#include "test.hpp"

using namespace std; 

int main(){
	// vector<string> v = {"dog", "bird", "lizard", "ant"}; 
	// // vector<char *> v2= {"dog", "bird", "lizard", "ant"};  
	// vector<char*> v2; 
	// for(int i = 0; i< 5; i++){
	// 	char word[] = "abcd"; 
	// 	v2.push_back(word);  
	// }
	//
	// list<string> l = {"dog", "bird", "lizard", "ant"}; 
	// int i[] = {1,5,1,2};  



	vector<string> vec(10, "");
	vector<char*> vec2(10, 0); 
	list<char*> l(10, 0); 
	int arr[10]; 

	random_number_generator rand_gen; 

	auto rand_word_object=[](random_number_generator& rand){return rand_word(rand);}; 
	auto rand_int_object = [](random_number_generator& rand){return rand_int(rand);}; 
	auto rand_char_array_object =
		[](random_number_generator& rand){return rand_char_array(rand);}; 

	fill_random(rand_gen, begin(vec), end(vec), rand_word_object);  
	fill_random(rand_gen, begin(vec2), end(vec2),  rand_char_array_object); 
	fill_random(rand_gen, begin(l), end(l),  rand_char_array_object); 
	fill_random(rand_gen, begin(arr), end(arr), rand_int_object); 

	// generalized_sort(v);
	// generalized_qsort(v); 
    //
	// generalized_sort(v2);
	// generalized_qsort(v2);
    //
	// generalized_sort(l);
	// generalized_qsort(l); 
    //
	// generalized_sort(i);
	// generalized_qsort(i); 
    //
    //
} 


