# Tau

Tau is a minimal example of a CMake project using [CNL](github.com/johnmcfarlane/cnl).
It was developed and tested in Debian 9.4 and built with Clang 6.0.0 and GCC 7.3.0.
It defines a 10-bit number representing *pi* and multiplies it by 2 to produce the value *tau*.

## Instructions

To build Tau in *~/tmp/tau* where CNL is cloned in *~/cnl*:

1. Configure CNL in a subdirectory.

       mkdir -p ~/tmp/tau/cnl
       cd ~/tmp/tau/cnl
       cmake -DCMAKE_INSTALL_PREFIX=~/tmp/tau ~/cnl

2. Build and install CNL.

       cmake --build . -- -j 8 install

3. Configure Tau.

       cd ~/tmp/tau
       cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=~/tmp/tau ~/cnl/example/tau

4. Build and run Tau.

       cmake --build .
       ./Tau

Output:

> 6.28125
