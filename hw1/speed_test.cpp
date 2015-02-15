#include <vector> 
#include <random> 
#include <iostream> 
#include <fstream>
#include <list> 
#include "custom_random.h"
#include <chrono> 
#include <utility> 

using namespace std; 

/* Inserts element into container, in the ordered position using linear search 
 * @param cont: the container into which the element is inserted
 * @param element: the element to be inserted
 * @param begin: iterator for the first element in the sequence
 * @param end: iterator for one past the last element in the sequence
 * */ 
template<typename Container, typename Iter, typename ElementType> 
void linear_insert(Container& cont, Iter begin, Iter end, ElementType element){
	while(begin != end && *begin<element){
		++begin;
	}
	cont.insert(begin, element);  
} 


/* Fills container with random, uniformly distributed numbers, in order.
 * @param cont: the container to be filled
 * @param num: the number of random numbers to fill container
 * @param seed: the seed with which to seed the random number generator
 * @param min: the minimum value of randomly generated numbers
 * @param max: the maximum value of randomly generate numbers*/
template<typename Container> 
void fill_random(Container& cont, int num, random_number_generator& rand_gen){
	rand_gen.set_default_max(); 
	for(int i=0; i<num; i++) linear_insert(cont, cont.begin(), cont.end(), rand_gen());
}

template<typename Container> 
void empty_random(Container& cont, random_number_generator& rand_gen){
	int size = cont.size(); 
	int offset; 

	for(int size = cont.size(); size > 0; size--){
		rand_gen.set_max(size-1);
		auto iter = cont.begin(); 
		offset = rand_gen(); 
		for(int i=0; i<offset; ++i) {
			++iter;
		}
		cont.erase(iter);  
	}

}

/* Prints the elements between the iterators. Debugging only. */
template<typename Iter> 
void print_contents(Iter begin, Iter end){
	while(begin!=end) cout << *begin++ << " "; 
	cout << endl;
} 

template<typename Iter> 
void print_pairs(Iter begin, Iter end, ofstream& file){
	// while(begin!=end) file << "Number: " << (*begin).first << "\tMilliseconds: "<< (*begin++).second << endl; 
	while(begin!=end) file << (*begin).first << "\t" << (*begin++).second << endl; 
	cout << endl;
}

template<typename Container> 
int time_insertion(Container& c, random_number_generator& rand_gen, int num){
	cout<< "Timing for " << num << " elements.\n"; 
	using std::chrono::system_clock; 
	using std::chrono::microseconds;
	
	auto start = system_clock::now(); 
	fill_random(c, num, rand_gen);
	empty_random(c, rand_gen);
	auto stop = system_clock::now(); 

	return std::chrono::duration_cast<microseconds>(stop - start).count();
}

int main(){
	int step = 10000; 
	int time; 
	vector<int> vec;
	vector<pair<int, int>> vec_time_list; 


	list<int> l;
	vector<pair<int, int>> list_time_list; 

	random_number_generator vec_gen(0, 0, 10000);
	random_number_generator list_gen(0, 0, 10000); 
	
	for(int num_elements= 1000; num_elements< 125000; num_elements+=step){
		time = time_insertion(vec, vec_gen, num_elements);
		pair<int, int> pair1{num_elements, time}; 
		vec_time_list.push_back(pair1); 	
		
		time = time_insertion(l, list_gen, num_elements);
		pair<int, int> pair2{num_elements, time};
		list_time_list.push_back(pair2); 
	}
	
	ofstream file;
	file.open("vector.txt"); 
	print_pairs(vec_time_list.begin(), vec_time_list.end(), file);

	ofstream file2; 
	file2.open("list.txt");
	print_pairs(list_time_list.begin(), list_time_list.end(), file2); 
}


