This program reads and evaluates a Makefile very much like the program make but with some simplifications, and the option to evaluate dependencies in parallel.

parse.c constructs a linked data structure by reading the makefile.

run_make.c evaluates and executes actions. the function run_make takes a target and finds the rules data structure corresponding with it then evaluates it. 

