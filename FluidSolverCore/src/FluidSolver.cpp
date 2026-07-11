// FluidSolver.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "FluidSolver.h"

extern "C" _declspec(dllexport) const unsigned int Echo(const unsigned int number) {
	return number * 10;
}
