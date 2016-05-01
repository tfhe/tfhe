#ifndef LWEPARAMS_H
#define LWEPARAMS_H

//this structure contains LWE parameters
//this structure is constant (cannot be modified once initialized): 
//the pointer to the param can be passed directly
//to all the LWE keys that use these params.
struct LWEParams {
	const int n;
	const double alpha_min;//le plus petit bruit tq sur
	const double alpha_max;//le plus gd bruit qui permet le déchiffrement



        //since all members are declared constant, a constructor is 
        //required in the structure.
#ifdef __cplusplus
	LWEParams(int n, double alpha_min, double alpha_max);
	~LWEParams();
	LWEParams(const LWEParams&) = delete; //forbidden
	LWEParams& operator=(const LWEParams& ) = delete; //forbidden
#endif
};

#endif //LWEPARAMS_H
