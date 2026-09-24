#include "FluidSimulation.h"

#include "math/operators/Staggered.h"
#include "domain/Domain.h"

#include "utils/profiling/ScopeProfiler.h"

#include "steps/ExternalForces.h"


void FluidSimulation::step(float timeStep) {
	ScopeProfiler p{ "============= COMPLETE SIMULATION STEP =============" };

	m_domain.extrapolateVelocity(m_velocityField, 3);
	m_advection.execute(m_domain, m_velocityField, timeStep);
	m_domain.updateCellData();

	if (m_applyGravity) ExternalForces::applyGravity(m_velocityField, m_domain, timeStep);
	
	m_advection.execute(m_smokeField, m_velocityField, m_domain, timeStep);
	
	m_domain.applyVelocityBoundaryConditions(m_velocityField);
	
	m_advection.execute(m_velocityField, m_domain, timeStep);
	m_diffusion.execute(m_velocityField, m_domain, m_kinematicViscosity, timeStep, m_iterationCount);
	m_pressureSolver.solveJacobi(m_pressureField, m_velocityField, m_domain, m_density, timeStep, m_iterationCount);
	m_projection.execute(m_velocityField, m_pressureField, m_domain, m_density, timeStep);
	
	Staggered::divergence(m_divergenceField, m_velocityField);
}
