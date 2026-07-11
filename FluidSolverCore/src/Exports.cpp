#include "FluidSimulation.h"

extern "C" _declspec(dllexport) FluidSimulation* CreateSimulation() {
	return new FluidSimulation();
}

extern "C" _declspec(dllexport) void DestroySimulation(FluidSimulation* handle) {
	delete handle;
}

extern "C" _declspec(dllexport) bool IsRunning(FluidSimulation* handle) {
	if (handle == nullptr) return false;
	return handle->running();
}

extern "C" _declspec(dllexport) void Stop(FluidSimulation* handle) {
	if (handle == nullptr) return;
	handle->stop();
}
