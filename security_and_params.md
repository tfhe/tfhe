---
layout: page
title: TFHE - Security and Parameters
previousPage: gate-bootstrapping-api.html
---

# Security estimates and parameter choices.

The current parameters implemented in the TFHE library have been updated from the ones proposend in the original TFHE paper [CGGI16], 
according to the new estimates done in the JoC paper [CGGI19], and new attack models integrated in [<span>LWE estimator</span>](https://bitbucket.org/malb/lwe-estimator/src/master/){:target="_blank"}. 
The implementation uses two sets of keys on two different noise levels, both required to execute the gate bootstrapping. 

|                                 | ciphertext dimension `n`	  | noise rate (stdev) `sd`      | security bits $\lambda$ |
| ------------------------------- | ------- | --------- | -------------- |
| Key-Switching key (LWE)         | 630     | $2^{-15}$ | 128 bits     |
| Bootstrapping key (Ring-LWE)    | 1024    | $2^{-25}$ | 130 bits	 |
| **Overall security**            |         |           | 128 bits       |

With these parameters, the gate bootstrapping runs in about `10-20 ms`, depending on the machine: as instance, one bootstrapped binary gate takes about `13 ms` on a Intel i9-9900k CPU and about `17 ms` on an average i7 Xeon processor (single core).

Our security estimates are made by using the [<span>LWE estimator</span>](https://bitbucket.org/malb/lwe-estimator/src/master/){:target="_blank"}. 
The estimates can change according to the new attacks proposed in the litterature and the updates of the estimator itself.
If you want to use safe parameters on the library in production, please double check the estimates and update your code with the new parameters.

The code to use in the LWE estimator to estimate hardness for the standard deviation `sd` ($2^{-25}$ in the example) and dimension `n` 
(1024 in the example) is provided below. We recommend to target at least 128-bits of security. 
In our implementation, we use 32 bits integers (`q=2**32`) and binary keys. 
For the choice of all the other TFHE parameters, please refer to the noise formulas in [CGGI19].


_Note:_ we estimate the parameters by using some of the models listed in the [<span>Estimate all the LWE and NTRU schemes</span>](https://estimate-all-the-lwe-ntru-schemes.github.io/docs/){:target="_blank"}.
In particular, we consider the classical cost of BKZ-beta on a lattice of dimension `d` to be `2^(0.292*beta + 16.4 + log(8*d,2))`.
To obtain more conservative parameters, we suggest using the core-SVP methodology using classical cost `2^(0.292*beta)` and quantum cost `2^(0.265*beta)`.



## ESTIMATOR CODE

```python
# To reproduce the estimate run this snippet on http://aleph.sagemath.org/
from sage.all import load, sqrt, RR, ZZ, pi, oo
load('https://bitbucket.org/malb/lwe-estimator/raw/HEAD/estimator.py')

n = 1024                 # ciphertext dimension (also, key entropy)
sd = 2**(-25)            # noise standard deviation
alpha = sqrt(2*pi)*sd    # estimator defines noise rate = sqrt(2pi).stdev
q = 2**32                # for compatibility only
m = oo                   # the attacker can use as many samples he wishes 
secret_distribution = (0,1)
success_probability = 0.99


# Chosen cost model 
# BKZ cost models: CLASSICAL - 0.292*beta + 16.4 + log(8*d,2) - primal
# i.e. BKZ.sieve =  lambda beta, d, B: ZZ(2)**RR(0.292*beta + 16.4 + log(8*d,2))
print("CLASSICAL PRIMAL")
print(primal_usvp(n, alpha, q, secret_distribution=secret_distribution, m=m, success_probability=success_probability, reduction_cost_model=BKZ.sieve))
# BKZ cost models: CLASSICAL - 0.292*beta + 16.4 + log(8*d,2) - dual
# i.e. BKZ.sieve =  lambda beta, d, B: ZZ(2)**RR(0.292*beta + 16.4 + log(8*d,2))
print("CLASSICAL DUAL")
print(dual_scale(n, alpha, q, secret_distribution=secret_distribution, m=m, success_probability=success_probability, reduction_cost_model=BKZ.sieve))


# For more conservative parameters, both classical and quantum  
# BKZ cost models: CLASSICAL - 0.292 beta - primal
reduction_cost_model =  lambda beta, d, B: ZZ(2)**RR(0.292*beta)
print("CLASSICAL PRIMAL (conservative)")
print(primal_usvp(n, alpha, q, secret_distribution=secret_distribution, m=m, success_probability=success_probability, reduction_cost_model=reduction_cost_model))
# BKZ cost models: CLASSICAL - 0.292 beta - dual
print("CLASSICAL DUAL (conservative)")
print(dual_scale(n, alpha, q, secret_distribution=secret_distribution, m=m, success_probability=success_probability, reduction_cost_model=reduction_cost_model))
# BKZ cost models: QUANTUM - 0.265 beta - primal
reduction_cost_model =  lambda beta, d, B: ZZ(2)**RR(0.265*beta)
print("QUANTUM PRIMAL (conservative)")
print(primal_usvp(n, alpha, q, secret_distribution=secret_distribution, m=m, success_probability=success_probability, reduction_cost_model=reduction_cost_model))
# BKZ cost models: QUANTUM - 0.265 beta - dual
print("QUANTUM DUAL (conservative)")
print(dual_scale(n, alpha, q, secret_distribution=secret_distribution, m=m, success_probability=success_probability, reduction_cost_model=reduction_cost_model))
```

_We would like to thank [<span>Fernando Virdia</span>](https://fundamental.domains/){:target="_blank"} for the help in the estimation of the security parameters._

