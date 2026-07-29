#include "FluidSimulation.h"
#include "math/operators/Staggered.h"
#include "math/solvers/PressureSolver.h"


void FluidSimulation::step(float timeStep) {
	m_advection.execute(m_velocityField, m_solidCellMap, timeStep);

	m_diffusion.execute(m_velocityField, timeStep);
	
	m_pressureSolver.solveJacobi(m_pressureField, m_density, timeStep, m_iterationCount);
	m_projection.execute(m_velocityField, m_pressureField, m_solidCellMap, m_density, timeStep);
	Staggered::divergence(m_divergenceField, m_velocityField);

	m_advection.execute(m_smokeField, m_velocityField, m_solidCellMap, timeStep);
}
