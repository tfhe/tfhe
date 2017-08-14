# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [1.0.1] - 2017-08-15
### Added
- Compatibility with MacOS, CentOS (clang compiler with libc++).
- Compatibility with Linux (g++ >= 5.2 and clang >=3.8 compilers with either libstdc++ or libc++).

### Changed
- All assembly sources are now position independent.
- libtfhe is compiled as a shared library
- optim and release builds use -O3 instead of -O2
- googletest is now a submodule of the repository (do 
  ```git submodule init; git submodule update``` 
  once before compiling the tests


## [1.0] - 2017-05-02
### Added
- The first version of TFHE supports symmetric encryption, decryption, and evaluation of a boolean circuit in Gate bootstrapping mode only (one bootstrapping per binary gate), and in single threaded mode.
- Every binary gate is supported: And, Or, Nand, Nor, Xor, XNor, Not, as well as the Mux gate.
- The current throughput of the library is one binary gate every 13ms on a core-i7 (4910MQ at 2.90GHz), independently of the nature of the gate.
