#include <vector> 
#include <random> 
#include <iostream> 
#include <fstream>
#include <list> 
#include <chrono> 
#include <utility> 
#include "custom_random.hpp"

using namespace std; 

namespace test{
	class large_container{
		int number;
		char a[50];

		public:
		large_container(int num):number{num}{
		
		};

		friend bool operator<(const large_container& lhs, const large_container& rhs){
			return lhs.number<rhs.number; 
		}
	};
	
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
	template<typename Type, typename Container> 
	void fill_random(Container& cont, vector<Type>& fill_vec){	
		int num = fill_vec.size(); 
		for(int i=0; i<num; i++) linear_insert(cont, cont.begin(), cont.end(), fill_vec[i]);  
	}

	template<typename Container> 
	void empty_random(Container& cont, vector<int>& empty_vec){
		int size = cont.size(); 
		int offset; 	

		for(int i = 0; i < size; i++){
			auto iter = cont.begin(); 
			offset = empty_vec[i];  
			for(int j=0; j<offset; ++j) ++iter;
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

	template<typename Type> 
	vector<Type> generate_fillers(int num, random_number_generator& rand_gen){
		rand_gen.set_default_max(); 
		vector<Type> fillers; 
		for(int i = 0; i<num; i++) fillers.push_back(rand_gen()); 
		return fillers; 
	}

	vector<int> generate_removal_indices(int size, random_number_generator& rand_gen){
		vector<int> indices; 
			
		while(size > 0){
			rand_gen.set_max(size-1);
			indices.push_back(rand_gen());
			size--; 
		}

		return indices; 
	} 

	template<typename Type, typename Container> 
	int time_insertion(Container& c, vector<Type> fillers, vector<int> removal_indicies){
		cout<< "Timing for " << fillers.size() << " elements.\n"; 
			
		using std::chrono::system_clock; 
		using std::chrono::microseconds;	
		auto start = system_clock::now(); 
		fill_random<Type>(c, fillers);
		empty_random(c, removal_indicies);
		auto stop = system_clock::now(); 

		return std::chrono::duration_cast<microseconds>(stop - start).count();
	}

	template<typename Type> 
	void run_tests(int start, int end, int step, string name1, string name2, int seed){
		//Vectors in which to store the results of timings. 
		//Pairs map number of elements to time
		vector<pair<int, int>> vec_time_list; 	
		vector<pair<int, int>> list_time_list;
			
		vector<Type> vec;
		list<Type> l;
		
		random_number_generator rand_gen(seed, 0, 10000);
		
		for(int num_elements = start; num_elements<= end; num_elements += step){
			vector<Type> fillers = generate_fillers<Type>(num_elements, rand_gen); 
			vector<int> removal_indicies = generate_removal_indices(num_elements, rand_gen); 

			int time; 

			time = time_insertion<Type>(vec, fillers, removal_indicies);
			pair<int, int> pair1{num_elements, time}; 
			vec_time_list.push_back(pair1); 	
			
			time = time_insertion<Type>(l, fillers, removal_indicies);
			pair<int, int> pair2{num_elements, time};
			list_time_list.push_back(pair2); 
		}
		
		ofstream file;
		file.open(name1); 
		print_pairs(vec_time_list.begin(), vec_time_list.end(), file);

		ofstream file2; 
		file2.open(name2);
		print_pairs(list_time_list.begin(), list_time_list.end(), file2); 

	} 

}
