all:
	clang++ src/cpu.cpp src/common.cpp src/kernel_functions.cpp src/timer.cpp src/optimizations.cpp -o ps1.cpp -O0 -g -Wall
