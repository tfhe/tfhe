#ifndef LWEPARAMS_H
#define LWEPARAMS_H

//this structure contains LWE parameters
//this structure is constant (cannot be modified once initialized): 
//the pointer to the param can be passed directly
//to all the LWE keys that use these params.
struct LWEParams {
	const int n;
	const double alpha;
	const int log_alpha;

        //since all members are declared constant, a constructor is 
        //required in the structure.
	LWEParams(int n, double alpha, int log_alpha);
};

//allocates and initialize lweParams
LWEParams* new_lweParams(int n, double alpha);

//destructs and frees a lweParams structure
void delete_lweParams(LWEParams* ptr);

#endif //LWEPARAMS_H
