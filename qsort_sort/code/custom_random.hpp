#ifndef __custom_random_h__
#define __custom_random_h__

#include <random> 
using namespace std; 

/* This is my custom random number generator. I use a default random engine and feed its output
 * to a uniform distribution. The purpose of having a class like this is to preserve the random 
 * number generator and not reseed it between trials. */

class random_number_generator{
	private:
		const static int DEFAULT_MAX = 10000;  
		default_random_engine gen;
		uniform_int_distribution<> dis; 
		int min;
		int max;
	public:
		random_number_generator(): min{0}, max{DEFAULT_MAX}, dis(0, DEFAULT_MAX), gen() {
			seed(0);
		}

		random_number_generator(int s, const int minimum, const int maximum): 
			min{minimum}, max{maximum}, dis(minimum, maximum){
			seed(s); 
		}

		void set_default_max(){
			set_max(DEFAULT_MAX);  
		}
			
		void seed(int s){
			gen.seed(s); 
		}

		void set_min(int m){
			min = m; 
			reset_distribution(); 
		}
		
		void set_max(int m){
			max = m;
			reset_distribution(); 	 
		}

		//Reset the uniform distribution after altering the min or max. 
		void reset_distribution(){
			uniform_int_distribution<> newdis(min, max); 
			dis = newdis;	
		}

		//Operator() allows us to just do rand_gen() to acquire the next random number. 
		int operator()(){return dis(gen);}

};
#endif

