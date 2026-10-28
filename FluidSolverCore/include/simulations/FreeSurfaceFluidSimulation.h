#pragma once

#include "FluidSimulation.h"
#include "steps/ExternalForces.h"

class FreeSurfaceFluidSimulation : public FluidSimulation {
public:
	FreeSurfaceFluidSimulation(const FluidSimulationConfig& config) : FluidSimulation(config) {}

	virtual void step(float timeStep) override {
		updateCellData();
		FluidSimulation::step(timeStep);
	}


private:
	virtual void updateCellData() = 0;
};
