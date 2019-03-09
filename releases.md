---
layout: default
title: TFHE: current and future releases
---

# Versions and features

<div style="text-align: center">
<table>
<tr><th>Feature</th><th>v1.0</th><th style="color: gray">v2.0</th></tr>
<tr><th>Binary Gates FHE</th><td>✔</td><td style="color: gray">✔</td></tr>
<tr><th>Symmetric encryption</th><td>✔</td><td style="color: gray">✔</td></tr>
<tr><th>Public key encryption</th><td>-</td><td style="color: gray">✔</td></tr>
<tr><th>Multithreading</th><td>✔</td><td style="color: gray">✔</td></tr>
<tr><th>Leveled mode LHE</th><td>-</td><td style="color: gray">✔</td></tr>
<tr><th>Multibit Arithmetic</th><td>-</td><td style="color: gray">✔</td></tr>
<tr><th>Circuit bootstrapping FHE</th><td>-</td><td style="color: gray">✔</td></tr>
<tr><th>Chimera bridges to other schemes</th><td>-</td><td style="color: gray">✔</td></tr>
</table>
</div>


### Current version: 1.0 

The first version of TFHE supports symmetric encryption, decryption, and 
evaluation of a boolean circuit in Gate bootstrapping mode only 
(one bootstrapping per binary gate), and in single threaded mode.

Every binary gate is supported: And, Or, Nand, Nor, Xor, XNor, ..., as
well as the Mux gate.

The current throughput of the library is one binary gate every 13ms,
independently of the nature of the gate.


### Future version 2.0 

Version 2.0 should support both bootstrapped (FHE) and leveled (LHE) execution, as
well as ciphertext packing, and circuit bootstrapping. The new features and optimizations are described in [CGGI17]

It should also support public key encryption and implement the bridges Chimera described in [BGG18]. 


### References

[[CGGI17]](https://eprint.iacr.org/2017/430){:target="_blank"}: I. Chillotti, N. Gama, M. Georgieva, and M. Izabachène. *Improving TFHE: faster packed homomorphic
operations and efficient circuit bootstrapping*. Cryptology ePrint Archive, report 2017/430.

[[BGG18]](https://eprint.iacr.org/2018/758){:target="_blank"}: C. Boura, N. Gama, M. Georgieva: Chimera: a unified framework for B/FV, TFHE and HEAAN fully homomorphic encryption and predictions for deep learning. IACR Cryptology ePrint Archive 2018: 758 (2018).


