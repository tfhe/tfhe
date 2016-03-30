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
	LWEParams(int n, double alpha);
	LWEParams(const LWEParams&) = delete; //forbidden
	LWEParams& operator=(const LWEParams& ) = delete; //forbidden
};

#endif //LWEPARAMS_H
