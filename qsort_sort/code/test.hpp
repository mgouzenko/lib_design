#include <cstdlib> 
#include <algorithm>
#include <iterator> 

template<typename Container>
void generalized_sort(Container& cont){
	auto begin_itr = begin(cont); 
	auto end_itr = end(cont); 
	sort(begin_itr, end_itr);  
}


template<typename Container> 
void generalized_qsort(Container& cont){

}

