#include "FluidSimulation.h"
#include "simulations/SinglePhaseFluidSimulation.h"
#include "simulations/SmokeSimulation.h"
#include "simulations/FreeSurfaceFluidSimulation.h"

#include "utils/profiling/Profiler.h"
#include <omp.h>


#if defined(__CYGWIN__)
	#define INTERFACE_EXPORT __declspec(dllexport)
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(WINAPI_FAMILY)
	#define INTERFACE_EXPORT __declspec(dllexport)
#elif defined(__MACH__) || defined(__ANDROID__) || defined(__linux__) || defined(LUMIN)
	#define INTERFACE_EXPORT __attribute__ ((visibility ("default")))
#else
	#define INTERFACE_EXPORT
#endif

enum SimulationType : uint8_t { SinglePhase, Smoke, FreeSurface };
extern "C" INTERFACE_EXPORT FluidSimulation* CreateSimulation(FluidSimulationConfig simulationConfig, SimulationType type) {
	switch (type) {
		case SinglePhase: return new SinglePhaseFluidSimulation(simulationConfig); break;
		case Smoke: return new SmokeSimulation(simulationConfig); break;
		case FreeSurface: return new FreeSurfaceFluidSimulation(simulationConfig); break;
		default: throw std::runtime_error("Provided a non implemented simulation type");
	}
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

extern "C" INTERFACE_EXPORT const float* GetSmokeFieldPtr(SmokeSimulation* handle) {
	return handle->getSmokeField().getValuesPtr();
}

extern "C" INTERFACE_EXPORT const float* GetVolumeFractionFieldPtr(FreeSurfaceFluidSimulation* handle) {
	return handle->getVolumeFractionField().getValuesPtr();
}

extern "C" INTERFACE_EXPORT const CellData* GetCellDataPtr(FluidSimulation* handle) {
	return handle->getCellData().getValuesPtr();
}

extern "C" INTERFACE_EXPORT const MarkerParticle* GetMarkerParticlesPtr(FluidSimulation* handle) {
	return handle->getMarkerParticles().data();
}

extern "C" INTERFACE_EXPORT const int MarkerParticleCount(FluidSimulation* handle) {
	return static_cast<int>(handle->getMarkerParticles().size());
}

extern "C" INTERFACE_EXPORT const float* GetLevelSetPtr(FreeSurfaceFluidSimulation* handle) {
	return handle->getLevelSet().getValuesPtr();
}

extern "C" INTERFACE_EXPORT const InterfaceData* GetFluidInterfacePtr(FreeSurfaceFluidSimulation* handle) {
	return handle->getFluidInterface().getValuesPtr();
}


struct CellInfo {
	float velRight, velLeft, velTop, velBottom;
	Vec2f normal;
	float intercept, volumeFraction, fluxRight, fluxLeft, fluxTop, fluxBottom;
};
extern "C" INTERFACE_EXPORT const CellInfo GetCellInfo(FreeSurfaceFluidSimulation* handle, int i, int j, float timeStep) {
	using enum VectorComponent;
	CellInfo info{};

	const StaggeredVectorField2D& velocityField = handle->getVelocityField();
	info.velRight = velocityField.getEdgeValue(X, i, j);
	info.velLeft = velocityField.getEdgeValue(X, i - 1, j);
	info.velTop = velocityField.getEdgeValue(Y, i, j);
	info.velBottom = velocityField.getEdgeValue(Y, i, j - 1);

	const FluidInterface2D& fluidInterface = handle->getFluidInterface();
	InterfaceData interfaceData = fluidInterface.getValue(i, j);
	info.normal = interfaceData.normal;
	info.intercept = interfaceData.intercept;

	const ScalarField2D& volumeFractionField = handle->getVolumeFractionField();
	info.volumeFraction = volumeFractionField.getValue(i, j);
	float cellWidth = handle->getCellWidth();
	info.fluxRight = fluidInterface.computeFluxAt(X, i, j, velocityField, volumeFractionField, cellWidth, timeStep);
	info.fluxLeft = fluidInterface.computeFluxAt(X, i - 1, j, velocityField, volumeFractionField, cellWidth, timeStep);
	info.fluxTop = fluidInterface.computeFluxAt(Y, i, j, velocityField, volumeFractionField, cellWidth, timeStep);
	info.fluxBottom = fluidInterface.computeFluxAt(Y, i, j - 1, velocityField, volumeFractionField, cellWidth, timeStep);
	
	return info;
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


extern "C" INTERFACE_EXPORT void AddSmoke(SmokeSimulation* handle, int i, int j, float deltaSmoke) {
	handle->addSmoke(i, j, deltaSmoke);
}


extern "C" INTERFACE_EXPORT void SetCell(FluidSimulation* handle, int i, int j, const CellConfig& config) {
	handle->setCell(i, j, config);
}

extern "C" INTERFACE_EXPORT void FloodDomain(FluidSimulation* handle) {
	handle->floodDomain();
}

extern "C" INTERFACE_EXPORT void DrainDomain(FluidSimulation* handle) {
	handle->drainDomain();
}

extern "C" INTERFACE_EXPORT void CreateMarkerParticles(FluidSimulation* handle) {
	handle->createMarkerParticles();
}
