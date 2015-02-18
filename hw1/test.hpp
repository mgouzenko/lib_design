#include <vector> 
#include <iostream> 
#include <fstream>
#include <list> 
#include <chrono> 
#include <utility> 
#include "custom_random.hpp"

using namespace std; 

namespace test{
	/* Large container is just a class that contains an integer and a large junk array. 
	 * Comparisons are done between two large containers by comparing their number private 
	 * members. */
	class large_container{
		int number;
		const static int SIZE = 200; 
		char a[SIZE];

		public:
		large_container(int num):number{num}{
			for(int i=0; i<SIZE; i++) a[i] = (char) rand()*255; 
		};

		//Define operator< so that we can insert objects of this class into vectors/lists in order. 
		friend bool operator<(const large_container& lhs, const large_container& rhs){
			return lhs.number<rhs.number; 
		}
	};
	

	/* Inserts element into container, in the ordered position using linear search 
	 * cont: the container into which the element is inserted
	 * element: the element to be inserted
	 * begin: iterator for the first element in the sequence
	 * end: iterator for one past the last element in the sequence
	 * */ 
	template<typename Container, typename Iter, typename ElementType> 
	void linear_insert(Container& cont, Iter begin, Iter end, ElementType element){
		//Find the proper position
		while(begin != end && *begin<element){
			++begin;
		}

		//and insert the element there. 
		cont.insert(begin, element);  
	} 


	/* Fills cont with the elements contained in fill_vec, inserting elements in proper order.
	 * cont: the container to be filled
	 * fill_vec: the vector containing elements that are to be inserted into cont. */
	template<typename Type, typename Container> 
	void fill_random(Container& cont, vector<Type>& fill_vec){	
		int num = fill_vec.size(); 

		//Iterate through the fillers, inserting them in order into cont
		for(int i=0; i<num; i++) linear_insert(cont, cont.begin(), cont.end(), fill_vec[i]);  
	}


	/* Empties the container cont by deleting elements at the indices specified by empty_vec. 
	 * cont: the container to be emptied. 
	 * empty_vec: the vector containing positions to delete in cont. */
	template<typename Container> 
	void empty_random(Container& cont, vector<int>& empty_vec){
		int size = cont.size(); 
		int offset; 	

		for(int i = 0; i < size; i++){
			auto iter = cont.begin(); 			
			offset = empty_vec[i];  			//The ith element of empty_vec indicates where to delete from. 
			for(int j=0; j<offset; ++j) ++iter;	//Advance the iterator by the necessary offset	
			cont.erase(iter);  					//Perform the deletion. 
		}

	}

	/* Prints the elements between the iterators. Debugging only. */
	template<typename Iter> 
	void print_contents(Iter begin, Iter end){
		while(begin!=end) cout << *begin++ << " "; 
		cout << endl;
	} 

	/* Writes the pairs contained between the two iterators to file. 
	 * begin: the iterator that points to the beginning of the sequence to be written.
	 * end: the iterator that points to one past the end of the sequence to be written.
	 * file: the ofstream to which the sequence should be written.*/
	template<typename Iter> 
	void print_pairs(Iter begin, Iter end, ofstream& file){
		//Advance the beginning iterator, printing first the number of elements, and then the time. 
		while(begin!=end) file << (*begin).first << "\t" << (*begin++).second << endl; 
		cout << endl;
	}

	/* Generates fillers of type Type. This Type, for our purposes, is either int or large_container.
	 * num: the number of elements to create. 
	 * rand_gen: the custom random number generator that should create our necessary integers. */
	template<typename Type> 
	vector<Type> generate_fillers(int num, random_number_generator& rand_gen){
		rand_gen.set_default_max(); 
		vector<Type> fillers; 

		//If Type is a large_container, the integers output by rand_gen are automatically converted to large_containers. 
		for(int i = 0; i<num; i++) fillers.push_back(rand_gen()); 
		return fillers; 
	}

	/* Generates removal positions used to empty containers. Note that these positions must be accessed in order from 
	 * front to back of the vector to ensure that they are valid indices. 
	 * size: the size of the container that needs to be emptied. 
	 * rand_gen: the custom random number generator that should be used to create positions.*/
	vector<int> generate_removal_indices(int size, random_number_generator& rand_gen){
		vector<int> indices; 

		while(size > 0){
			rand_gen.set_max(size-1);		
			indices.push_back(rand_gen());	
			size--; 						//The max size needs to decrease on each iteration to ensure valid removal indices. 
		}

		return indices; 
	} 

	
	/* Times the insertion and deletion of elements from the container supplied as c. 
	 * c: the container to be inserted into and deleted from 
	 * fillers: a vector of fillers (ints or large_containers) to be inserted. 
	 * removal_indices: the indices from which to remove elements. */
	template<typename Type, typename Container> 
	int time_insertion(Container& c, vector<Type> fillers, vector<int> removal_indicies){
		cout<< "Timing for " << fillers.size() << " elements" <<endl; 
			
		using std::chrono::system_clock; 
		using std::chrono::microseconds;

		auto start = system_clock::now(); 
		fill_random<Type>(c, fillers);
		empty_random(c, removal_indicies);
		auto stop = system_clock::now(); 

		return std::chrono::duration_cast<microseconds>(stop - start).count();
	}


	/* The test driver, which performs the experiment for a varying number of elements. 
	 * start: the starting number of elements. 
	 * end: the ending number of elements.
	 * step: the amount to increment per experiment.
	 * name1: the name of the vector outfile. 
	 * name2: the name of the list outfile. 
	 * seed: the seed with which to seed the random number generator.*/
	template<typename Type> 
	void run_tests(int start, int end, int step, string name1, string name2, int seed){
		//Vectors in which to store the results of timings. Pair elements map number of elements to time.
		vector<pair<int, int>> vec_time_list; 	
		vector<pair<int, int>> list_time_list;
			
		vector<Type> vec;
		list<Type> l;
		
		random_number_generator rand_gen(seed, 0, 10000);
		
		for(int num_elements = start; num_elements<= end; num_elements += step){
			//Use the same fillers and removal indices for lists and vectors. 
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
		

		//Print vector results
		ofstream file;
		file.open(name1); 
		print_pairs(vec_time_list.begin(), vec_time_list.end(), file);


		//Print list results
		ofstream file2; 
		file2.open(name2);
		print_pairs(list_time_list.begin(), list_time_list.end(), file2); 

	} 

}
