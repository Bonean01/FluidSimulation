#include "FluidSimulation.h"

#include "math/operators/Staggered.h"
#include "domain/BoundaryUtils.h"

#include "utils/profiling/ScopeProfiler.h"

#include <omp.h>
void FluidSimulation::step(float timeStep) {
	{
		ScopeProfiler p{ "Velocity BCs" };
		BoundaryUtils::applyVelocityBoundaryConditions(m_velocityField, m_boundaryData);
	}
	{
		ScopeProfiler p{ "Self-Advection" };
		m_advection.execute(m_velocityField, m_boundaryData, timeStep);
	}
	{
		ScopeProfiler p{ "Diffusion" };
		m_diffusion.execute(m_velocityField, m_boundaryData, m_kinematicViscosity, timeStep, m_iterationCount);
	}
	{
		ScopeProfiler p{ "Pressure solve" };
		m_pressureSolver.solveJacobi(m_pressureField, m_velocityField, m_cellData, m_density, timeStep, m_iterationCount);
	}
	{
		ScopeProfiler p{ "Projection" };
		m_projection.execute(m_velocityField, m_pressureField, m_boundaryData, m_density, timeStep);
	}
	{
		ScopeProfiler p{ "Smoke field advection" };
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
