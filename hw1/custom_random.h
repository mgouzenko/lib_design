#ifndef __custom_random_h__
#define __custom_random_h__
using namespace std; 

class random_number_generator{
	private:
		const static int DEFAULT_MAX = 2000;  
		default_random_engine gen;
		uniform_int_distribution<> dis; 
		int rand_seed;
		int min;
		int max;
	public:
		random_number_generator(): rand_seed{0}, min{0}, max{2000}, dis(0,2000) {
		}

		random_number_generator(int s, const int minimum, const int maximum): 
			rand_seed{s}, min{minimum}, max{maximum}, dis(minimum, maximum){
		}

		void set_default_max(){
			set_max(DEFAULT_MAX);  
		}
			
		void seed(int s){
			rand_seed = s; 
		}

		void set_min(int m){
			min = m; 
			reset_distribution(); 
		}
		
		void set_max(int m){
			max = m;
			reset_distribution(); 	 
		}

		void reset_distribution(){
			uniform_int_distribution<> newdis(min, max); 
			dis = newdis;	
		}

		int operator()(){return dis(gen);}

};
#endif

