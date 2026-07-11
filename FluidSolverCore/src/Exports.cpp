#include "FluidSimulation.h"

extern "C" _declspec(dllexport) FluidSimulation* CreateSimulation(int width, int height) {
	return new FluidSimulation(width, height);
}

extern "C" _declspec(dllexport) void DestroySimulation(FluidSimulation* handle) {
	delete handle;
}
