#include "steps/Advection.h"

#include <omp.h>
#include <cmath>

#include "utils/profiling/ScopeProfiler.h"
#include "domain/DomainUtils.h"


void Advection::execute(StaggeredVectorField2D& velocityField, const StaggeredGrid2D<BoundaryData>& boundaryData, float timeStep) {
	ScopeProfiler p{ "Self Advection" };
	using enum VectorComponent;

	int width = velocityField.width();
	int height = velocityField.height();
	if (m_auxStaggeredVectorField.width() != width || m_auxStaggeredVectorField.height() != height) return;

	advectComponent(X, velocityField, boundaryData, timeStep);
	advectComponent(Y, velocityField, boundaryData, timeStep);

	std::swap(velocityField, m_auxStaggeredVectorField);
}


void Advection::advectComponent(const VectorComponent& C, StaggeredVectorField2D& velocityField, const StaggeredGrid2D<BoundaryData>& boundaryData, float timeStep) {
	int width = velocityField.getValuesWidth(C);
	int height = velocityField.getValuesHeight(C);
	
	#pragma omp parallel for
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			const BoundaryData& currentBoundary = boundaryData.getEdgeValue(C, i, j);
			if (DomainUtils::hasBoundaryPrescribedVelocity(currentBoundary)) continue;

			Vec2f position = velocityField.getEdgePosition(C, i, j);
			Vec2f currentVel = velocityField.sampleBilinear(position);
			Vec2f newValue = velocityField.sampleBilinear(position - currentVel * timeStep);
			m_auxStaggeredVectorField.setEdgeValue(C, i, j, newValue.get(C));
		}
	}
}


void Advection::execute(ScalarField2D& field, const StaggeredVectorField2D& velocityField, const Grid2D<CellData>& cellData, float timeStep) {
	ScopeProfiler p{ "Scalar Field Advection" };

	int width = field.width();
	int height = field.height();
	float dx = field.cellWidth();
	if (m_auxScalarField.width() != width || m_auxScalarField.height() != height) return;

	#pragma omp parallel for
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			if (cellData.getValue(i, j).cellType == CellType::Solid) { m_auxScalarField.setValue(i, j, 0.0f); continue; }
			Vec2f position = { (float)i, (float)j };
			Vec2f currentVel = velocityField.sampleBilinear(position);
			float newValue = field.sampleBilinear(position - currentVel / dx * timeStep);
			m_auxScalarField.setValue(i, j, newValue);
		}
	}
	std::swap(field, m_auxScalarField);
}


// Uses second order Runge-Kutta advection to advect marker particles based 
// on the extrapolated velocity field, takes care of collisions with solids
void Advection::execute(std::vector<MarkerParticle>& markerParticles, const StaggeredVectorField2D& velocityField, const Grid2D<CellData>& cellData, float timeStep) {
	ScopeProfiler p{ "Marker Particles Advection" };

	float dx = velocityField.cellWidth();
	
	#pragma omp parallel for
	for (int i = 0; i < markerParticles.size(); i++) {
		MarkerParticle& particle = markerParticles.at(i);
		Vec2f& currentPos = particle.position;
		Vec2f currentVel = velocityField.sampleBilinear(currentPos);
		
		Vec2f finalPos = particle.position + currentVel / dx * timeStep;
		Vec2f finalVel = velocityField.sampleBilinear(finalPos);
		
		Vec2f averageVel = (currentVel + finalVel) / 2;
		Vec2f newPos = particle.position + averageVel / dx * timeStep;

		const CellData& finalCell = cellData.getValue(static_cast<int>(std::floor(newPos.x)), static_cast<int>(std::floor(newPos.y)));
		if (finalCell.cellType != CellType::Solid)
			particle.position = newPos;
	}
}
