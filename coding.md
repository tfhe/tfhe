---
layout: page
title: TFHE Fast Fully Homomorphic Encryption over the Torus
---

[back: using the library](usage.html) - 
[home](index.html) -
[Step 1: alice.c](tuto-alice.html) - 
[Step 2: cloud.c](tuto-cloud.html) - 
[Step 3: verif.c](tuto-verif.html) - 
[the full API](gate-bootstrapping-api.html) 

# A hello world example

In this hello world example, we will walk you through a simple use case.
1. First, a client Alice will generate keys and encrypt two numbers.
2. Then, the cloud will do some homomorphic computations: here, it will
   compute the minimum of the two numbers.
3. Finally, Alice will decrypt and print the cloud's answer

Each of these three steps will be coded as a separate binary, and data that
should be transmitted between programs will be saved and loaded in files.

For a quick introduction, you may download the three ready to compile
programs [here](asset/data/samples.tar.gz) and follow the explanations on
the next pages.

[Step 1: alice.c](tuto-alice.html) - 
[Step 2: cloud.c](tuto-cloud.html) - 
[Step 3: verif.c](tuto-verif.html) - 
[the full API](gate-bootstrapping-api.html) 

