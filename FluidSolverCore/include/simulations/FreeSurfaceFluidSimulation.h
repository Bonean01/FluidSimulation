#pragma once

#include "FluidSimulation.h"
#include "steps/ExternalForces.h"

class FreeSurfaceFluidSimulation : public FluidSimulation {
public:
	FreeSurfaceFluidSimulation(const FluidSimulationConfig& config) : FluidSimulation(config) {}

	virtual void step(float timeStep) {
		m_advection.execute(m_markerParticles, m_velocityField, m_domain, timeStep);

		m_domain.applyVelocityBoundaryConditions(m_velocityField);

		m_advection.execute(m_velocityField, m_domain, timeStep);
		m_diffusion.execute(m_velocityField, m_domain, m_kinematicViscosity, timeStep, m_iterationCount);
		m_pressureSolver.solveJacobi(m_pressureField, m_velocityField, m_domain, m_density, timeStep, m_iterationCount);
		m_projection.execute(m_velocityField, m_pressureField, m_domain, m_density, timeStep);

		Staggered::divergence(m_divergenceField, m_velocityField);
	}
};
