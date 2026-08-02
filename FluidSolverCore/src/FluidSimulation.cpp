#include "FluidSimulation.h"
#include "math/operators/Staggered.h"
#include "math/solvers/PressureSolver.h"


void FluidSimulation::step(float timeStep) {
	m_advection.execute(m_velocityField, m_cellData, timeStep);

	m_diffusion.execute(m_velocityField, m_kinematicViscosity, timeStep, m_iterationCount);
	
	m_pressureSolver.solveJacobi(m_pressureField, m_velocityField, m_cellData, m_density, timeStep, m_iterationCount);
	m_projection.execute(m_velocityField, m_pressureField, m_density, timeStep);
	Staggered::divergence(m_divergenceField, m_velocityField);

	m_advection.execute(m_smokeField, m_velocityField, m_cellData, timeStep);
}
