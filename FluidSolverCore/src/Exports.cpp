#include "FluidSimulation.h"

// TODO: Add preprocessor directives to set the appropriate export depending on the platform

extern "C" _declspec(dllexport) FluidSimulation* CreateSimulation(int width, int height, float cellWidth, float density, float kinematicViscosity, unsigned int iterationCount) {
	return new FluidSimulation(width, height, cellWidth, density, kinematicViscosity, iterationCount);
}

extern "C" _declspec(dllexport) void DestroySimulation(FluidSimulation* handle) {
	delete handle;
}


extern "C" _declspec(dllexport) void Step(FluidSimulation* handle, float dt) {
	handle->step(dt);
}


extern "C" _declspec(dllexport) float GetProjectionError(FluidSimulation* handle) {
	return handle->getProjectionError();
}



//extern "C" _declspec(dllexport) const Vec2f* GetVelocityFieldPtr(FluidSimulation* handle) {
//	return handle->getVelocityField().getValuesPtr();
//}

extern "C" _declspec(dllexport) const float* GetPressureFieldPtr(FluidSimulation* handle) {
	return handle->getPressureField().getValuesPtr();
}

extern "C" _declspec(dllexport) const float* GetDivergenceFieldPtr(FluidSimulation* handle) {
	return handle->getDivergenceField().getValuesPtr();
}

extern "C" _declspec(dllexport) const uint8_t* GetSolidCellMapPtr(FluidSimulation* handle) {
	return handle->getSolidCellMap().getValuesPtr();
}

extern "C" _declspec(dllexport) void SetVelocity(FluidSimulation* handle, int i, int j, Vec2f velocity) {
	handle->setVelocity(i, j, velocity);
}

struct CVec2f { float x, y; };
extern "C" _declspec(dllexport) CVec2f GetVelocity(FluidSimulation* handle, int i, int j) {
	Vec2f vel = handle->getVelocity(i, j);
	return CVec2f{ vel.x, vel.y };
}

extern "C" _declspec(dllexport) void AddVelocity(FluidSimulation* handle, int i, int j, Vec2f deltaVel) {
	handle->addVelocity(i, j, deltaVel);
}

extern "C" _declspec(dllexport) void SetSolidCell(FluidSimulation* handle, int i, int j, bool isSolid) {
	handle->setSolidCell(i, j, isSolid);
}
