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
	return handle->getVelocityField().getValuesPtr();
}

extern "C" _declspec(dllexport) const float* GetPressureFieldPtr(FluidSimulation* handle) {
	return handle->getPressureField().getValuesPtr();
}

extern "C" _declspec(dllexport) const uint8_t* GetSolidCellMapPtr(FluidSimulation* handle) {
	return handle->getSolidCellMap().getValuesPtr();
}


extern "C" _declspec(dllexport) void SetVelocity(FluidSimulation* handle, int i, int j, Vector2 velocity) {
	handle->setVelocity(i, j, velocity);
}

extern "C" _declspec(dllexport) void SetSolidCell(FluidSimulation* handle, int i, int j, bool isSolid) {
	handle->setSolidCell(i, j, isSolid);
}
