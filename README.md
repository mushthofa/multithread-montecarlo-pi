# multithread-montecarlo-pi
A simple example in C++ to demonstrate Boost multithreading library to calculate pi using the simple Monte-Carlo method

To compile, make sure that you have Boost library and CMake installed.
Then, in the directory where you put main.cpp and CMakeLists.txt, do

$ mkdir build
$ cd build
$ cmake ../
$ make

Then the executable 'pimc' will be created.
Simply call

$./pimc

to execute, or 

$./pimc --help

to see the possible options.

