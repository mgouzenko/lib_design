#include <list>
#include <vector> 
#include "test.hpp"

using namespace std; 

int main(){
	vector<string> v = {"dog", "bird", "lizard", "ant"}; 
	vector<char *> v2= {"dog", "bird", "lizard", "ant"};  
	list<string> l = {"dog", "bird", "lizard", "ant"}; 
	int i[] = {1,5,1,2};  


	generalized_sort(v);
	generalized_qsort(v); 

	generalized_sort(v2);
	generalized_qsort(v2);

	generalized_sort(l);
	generalized_qsort(l); 

	generalized_sort(i);
	generalized_qsort(i); 


} 


