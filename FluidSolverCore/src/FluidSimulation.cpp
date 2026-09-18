#include "FluidSimulation.h"

#include "math/operators/Staggered.h"
#include "domain/DomainUtils.h"

#include "utils/profiling/ScopeProfiler.h"

#include "steps/ExternalForces.h"


void FluidSimulation::step(float timeStep) {
	ScopeProfiler p{ "============= COMPLETE SIMULATION STEP =============" };

	m_surfaceSDF.update(m_markerParticles, 1);
	DomainUtils::extrapolateVelocity(m_velocityField, m_cellData);
	m_advection.execute(m_markerParticles, m_velocityField, m_cellData, timeStep);
	DomainUtils::updateCellData(m_cellData, m_markerParticles);

	m_advection.execute(m_smokeField, m_velocityField, m_cellData, timeStep);

	DomainUtils::applyVelocityBoundaryConditions(m_velocityField, m_boundaryData);

	m_advection.execute(m_velocityField, m_boundaryData, timeStep);
	m_diffusion.execute(m_velocityField, m_boundaryData, m_kinematicViscosity, timeStep, m_iterationCount);
	m_pressureSolver.solveJacobi(m_pressureField, m_velocityField, m_cellData, m_density, timeStep, m_iterationCount);
	m_projection.execute(m_velocityField, m_pressureField, m_boundaryData, m_density, timeStep);

	//ExternalForces::applyGravity(m_velocityField, m_boundaryData, timeStep);
}


void FluidSimulation::setCell(int i, int j, const CellData& cellData, const BoundaryData& boundaryData) {
	using enum VectorComponent;
	m_cellData.setValue(i, j, cellData);
	m_boundaryData.setEdgeValue(X, i, j, boundaryData);
	m_boundaryData.setEdgeValue(X, i + 1, j, boundaryData);
	m_boundaryData.setEdgeValue(Y, i, j, boundaryData);
	m_boundaryData.setEdgeValue(Y, i, j + 1, boundaryData);

	if (cellData.cellType == CellType::Fluid) {
		Vec2f cellCenter = Vec2f(i + 0.5f, j + 0.5f);
		m_markerParticles.emplace_back(MarkerParticle(Vec2f(cellCenter.x + 0.25f, cellCenter.y + 0.25f)));
		m_markerParticles.emplace_back(MarkerParticle(Vec2f(cellCenter.x - 0.25f, cellCenter.y - 0.25f)));
		m_markerParticles.emplace_back(MarkerParticle(Vec2f(cellCenter.x + 0.25f, cellCenter.y - 0.25f)));
		m_markerParticles.emplace_back(MarkerParticle(Vec2f(cellCenter.x - 0.25f, cellCenter.y + 0.25f)));
	}
}
