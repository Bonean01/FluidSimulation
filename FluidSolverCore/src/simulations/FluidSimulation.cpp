#include "simulations/FluidSimulation.h"

#include "math/operators/Staggered.h"
#include "domain/Domain.h"

#include "utils/profiling/ScopeProfiler.h"

#include "steps/ExternalForces.h"


void FluidSimulation::step(float timeStep) {
	ScopeProfiler p{ "============= COMPLETE SIMULATION STEP =============" };

	updateVelocityField();
	selfAdvect(timeStep);
	diffuse(timeStep);
	if (m_applyGravity)
		applyGravity(timeStep);
	pressureSolve(timeStep);
	project(timeStep);
	advectProperties(timeStep);
}


void FluidSimulation::updateVelocityField() {
	m_domain.applyVelocityBoundaryConditions(m_velocityField);
}

void FluidSimulation::selfAdvect(float timeStep) {
	m_advection.execute(m_velocityField, m_domain, timeStep);
}

void FluidSimulation::diffuse(float timeStep) {
	m_diffusion.execute(m_velocityField, m_domain, m_kinematicViscosity, timeStep, m_iterationCount);
}

void FluidSimulation::applyGravity(float timeStep) {
	ExternalForces::applyGravity(m_velocityField, m_domain, timeStep);
}

void FluidSimulation::pressureSolve(float timeStep) {
	m_pressureSolver.solveJacobi(m_pressureField, m_velocityField, m_domain, m_density, timeStep, m_iterationCount);
}

void FluidSimulation::project(float timeStep) {
	m_projection.execute(m_velocityField, m_pressureField, m_domain, m_density, timeStep);
}

void FluidSimulation::advectProperties(float timeStep) {
	m_advection.execute(m_markerParticles, m_velocityField, m_domain, timeStep);
}
