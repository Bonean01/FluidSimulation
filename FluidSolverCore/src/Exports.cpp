#include "FluidSimulation.h"

// TODO: Add preprocessor directives to set the appropriate export depending on the platform

extern "C" _declspec(dllexport) FluidSimulation* CreateSimulation(int width, int height, float cellWidth, float density, float kinematicViscosity) {
	return new FluidSimulation(width, height, cellWidth, density, kinematicViscosity);
}

extern "C" _declspec(dllexport) void DestroySimulation(FluidSimulation* handle) {
	delete handle;
}

extern "C" _declspec(dllexport) void Step(FluidSimulation* handle, float dt) {
	handle->step(dt);
}

extern "C" _declspec(dllexport) const Vector2* GetVelocityFieldPtr(FluidSimulation* handle) {
	return handle->getVelocityField()->getValuesPtr();
}

extern "C" _declspec(dllexport) int GetVelocityFieldSize(FluidSimulation* handle) {
	return handle->getVelocityField()->size();
}
