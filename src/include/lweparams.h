#ifndef LwePARAMS_H
#define LwePARAMS_H

//this structure contains Lwe parameters
//this structure is constant (cannot be modified once initialized): 
//the pointer to the param can be passed directly
//to all the Lwe keys that use these params.
struct LweParams {
	const int n;
	const double alpha_min;//le plus petit bruit tq sur
	const double alpha_max;//le plus gd bruit qui permet le déchiffrement



//since all members are declared constant, a constructor is 
//required in the structure.
#ifdef __cplusplus
	LweParams(int n, double alpha_min, double alpha_max);
	~LweParams();
	LweParams(const LweParams&) = delete; //forbidden
	LweParams& operator=(const LweParams& ) = delete; //forbidden
#endif
};

#endif //LwePARAMS_H
