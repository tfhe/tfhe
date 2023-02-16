FROM ubuntu:20.04

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
  clang \
  cmake \
  git

WORKDIR /root
RUN git clone https://github.com/tfhe/tfhe && \
  cd tfhe && \
  make && \
  make install
