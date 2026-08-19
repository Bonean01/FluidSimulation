#include "FluidSimulation.h"

#include "math/operators/Staggered.h"
#include "domain/BoundaryUtils.h"

#include "utils/profiling/ProfileScope.h"


void FluidSimulation::step(float timeStep) {
	{
		ProfileScope p{ "Velocity BCs" };
		BoundaryUtils::applyVelocityBoundaryConditions(m_velocityField, m_boundaryData);
	}
	{
		ProfileScope p{ "Self-Advection" };
		m_advection.execute(m_velocityField, m_boundaryData, timeStep);
	}
	{
		ProfileScope p{ "Diffusion" };
		m_diffusion.execute(m_velocityField, m_boundaryData, m_kinematicViscosity, timeStep, m_iterationCount);
	}
	{
		ProfileScope p{ "Pressure solve" };
		m_pressureSolver.solveJacobi(m_pressureField, m_velocityField, m_cellData, m_density, timeStep, m_iterationCount);
	}
	{
		ProfileScope p{ "Projection" };
		m_projection.execute(m_velocityField, m_pressureField, m_boundaryData, m_density, timeStep);
	}
	{
		ProfileScope p{ "Smoke field advection" };
		m_advection.execute(m_smokeField, m_velocityField, m_cellData, timeStep);
	}
}


void FluidSimulation::setCell(int i, int j, CellData cellData, BoundaryData boundaryData) {
	using enum VectorComponent;
	m_cellData.setValue(i, j, cellData);
	m_boundaryData.setEdgeValue(X, i, j, boundaryData);
	m_boundaryData.setEdgeValue(X, i + 1, j, boundaryData);
	m_boundaryData.setEdgeValue(Y, i, j, boundaryData);
	m_boundaryData.setEdgeValue(Y, i, j + 1, boundaryData);
}
