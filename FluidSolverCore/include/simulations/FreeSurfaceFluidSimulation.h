#pragma once

#include "FluidSimulation.h"

class FreeSurfaceFluidSimulation : public FluidSimulation {
public:
	FreeSurfaceFluidSimulation(const FluidSimulationConfig& config)
		: FluidSimulation(config),
		m_surfaceTensionCoefficient(config.surfaceTensionCoefficient) {}

	virtual void step(float timeStep) override {
		updateCellData();
		FluidSimulation::step(timeStep);
	}

	float getSurfaceTensionCoefficient() { return m_surfaceTensionCoefficient; }

protected:
	float m_surfaceTensionCoefficient;

private:
	virtual void updateCellData() = 0;
};
