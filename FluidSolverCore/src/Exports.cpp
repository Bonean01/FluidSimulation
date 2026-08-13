#include "FluidSimulation.h"


#if defined(__CYGWIN__)
	#define INTERFACE_EXPORT __declspec(dllexport)
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(WINAPI_FAMILY)
	#define INTERFACE_EXPORT __declspec(dllexport)
#elif defined(__MACH__) || defined(__ANDROID__) || defined(__linux__) || defined(LUMIN)
	#define INTERFACE_EXPORT __attribute__ ((visibility ("default")))
#else
	#define INTERFACE_EXPORT
#endif


extern "C" INTERFACE_EXPORT FluidSimulation* CreateSimulation(int width, int height, float cellWidth, float density, float kinematicViscosity, unsigned int iterationCount) {
	return new FluidSimulation(width, height, cellWidth, density, kinematicViscosity, iterationCount);
}

extern "C" INTERFACE_EXPORT void DestroySimulation(FluidSimulation* handle) {
	delete handle;
}


extern "C" INTERFACE_EXPORT void Step(FluidSimulation* handle, float timeStep) {
	handle->step(timeStep);
}


extern "C" INTERFACE_EXPORT const float* GetPressureFieldPtr(FluidSimulation* handle) {
	return handle->getPressureField().getValuesPtr();
}

extern "C" INTERFACE_EXPORT const float* GetDivergenceFieldPtr(FluidSimulation* handle) {
	return handle->getDivergenceField().getValuesPtr();
}

extern "C" INTERFACE_EXPORT const float* GetSmokeFieldPtr(FluidSimulation* handle) {
	return handle->getSmokeField().getValuesPtr();
}

extern "C" INTERFACE_EXPORT const CellData* GetCellDataPtr(FluidSimulation* handle) {
	return handle->getCellData().getValuesPtr();
}


extern "C" INTERFACE_EXPORT void SetVelocity(FluidSimulation* handle, int i, int j, Vec2f velocity) {
	handle->setVelocity(i, j, velocity);
}

struct CVec2f { float x, y; };
extern "C" INTERFACE_EXPORT CVec2f GetVelocity(FluidSimulation* handle, int i, int j) {
	Vec2f vel = handle->getVelocity(i, j);
	return CVec2f{ vel.x, vel.y };
}

extern "C" INTERFACE_EXPORT void AddVelocity(FluidSimulation* handle, int i, int j, Vec2f deltaVel) {
	handle->addVelocity(i, j, deltaVel);
}


extern "C" INTERFACE_EXPORT void AddSmoke(FluidSimulation* handle, int i, int j, float deltaSmoke) {
	handle->addSmoke(i, j, deltaSmoke);
}


extern "C" INTERFACE_EXPORT void SetCell(FluidSimulation* handle, int i, int j, CellProperties cellProperties) {
	handle->setCell(i, j, cellProperties);
}
