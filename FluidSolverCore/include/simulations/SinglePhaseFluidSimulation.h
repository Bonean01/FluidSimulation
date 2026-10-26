#pragma once

#include "FluidSimulation.h"

class SinglePhaseFluidSimulation : public FluidSimulation {
public:
	SinglePhaseFluidSimulation(const FluidSimulationConfig& config) : FluidSimulation(config) { m_domain.flood(); }
};
