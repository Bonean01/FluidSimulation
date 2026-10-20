#include "steps/Advection.h"

#include <omp.h>
#include <cmath>
#include <iostream>

#include "utils/profiling/ScopeProfiler.h"
#include "math/operators/Staggered.h"
#include "domain/Domain.h"


void Advection::execute(StaggeredVectorField2D& velocityField, const Domain& domain, float timeStep) {
	ScopeProfiler p{ "Self Advection" };
	using enum VectorComponent;

	int width = velocityField.width();
	int height = velocityField.height();
	if (m_auxStaggeredVectorField.width() != width || m_auxStaggeredVectorField.height() != height) return;

	advectComponent(X, velocityField, domain, timeStep);
	advectComponent(Y, velocityField, domain, timeStep);

	std::swap(velocityField, m_auxStaggeredVectorField);
}


void Advection::advectComponent(const VectorComponent& C, StaggeredVectorField2D& velocityField, const Domain& domain, float timeStep) {
	int width = velocityField.getValuesWidth(C);
	int height = velocityField.getValuesHeight(C);
	
	#pragma omp parallel for
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			const BoundaryData& currentBoundary = domain.getBoundary(C, i, j);
			if (domain.hasEdgePrescribedVelocity(C, i, j)) continue;
			//if (not domain.isFluidEdge(C, i, j)) continue;

			Vec2f position = velocityField.getEdgePosition(C, i, j);
			Vec2f currentVel = velocityField.sampleBilinear(position);
			Vec2f newValue = velocityField.sampleBilinear(position - currentVel * timeStep);
			m_auxStaggeredVectorField.setEdgeValue(C, i, j, newValue.get(C));
		}
	}
}


void Advection::execute(ScalarField2D& field, const StaggeredVectorField2D& velocityField, const Domain& domain, float timeStep) {
	ScopeProfiler p{ "Scalar Field Advection" };

	int width = field.width();
	int height = field.height();
	float dx = field.cellWidth();
	if (m_auxScalarField.width() != width || m_auxScalarField.height() != height) return;

	#pragma omp parallel for
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			if (domain.getCell(i, j).cellType == CellType::Solid) { m_auxScalarField.setValue(i, j, 0.0f); continue; }
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
void Advection::execute(std::vector<MarkerParticle>& markerParticles, const StaggeredVectorField2D& velocityField, const Domain& domain, float timeStep) {
	ScopeProfiler p{ "Marker Particles Advection" };

	float dx = velocityField.cellWidth();

	int n = 10;
	timeStep /= n;
	for (int k = 0; k < n; k++) {
		#pragma omp parallel for
		for (int i = 0; i < markerParticles.size(); i++) {
			MarkerParticle& particle = markerParticles.at(i);
			Vec2f& currentPos = particle.position;
			Vec2f currentVel = velocityField.sampleBilinear(currentPos);

			Vec2f finalPos = particle.position + currentVel / dx * timeStep;
			Vec2f finalVel = velocityField.sampleBilinear(finalPos);

			Vec2f averageVel = (currentVel + finalVel) / 2;
			Vec2f newPos = particle.position + averageVel / dx * timeStep;


			// Collision resolving
			Vec2f& collisionPos = currentPos;
			Vec2f correctedPos = newPos;

			if (domain.isPointInsideSolidCell(newPos))
				correctedPos = { newPos.x, collisionPos.y };
			
			if (domain.isPointInsideSolidCell(correctedPos))
				correctedPos = { collisionPos.x, newPos.y };

			if (not domain.isPointInsideSolidCell(correctedPos))
				particle.position = correctedPos;
		}
	}
}


void Advection::execute(ScalarField2D& volumeFractionField, FluidInterface2D& fluidInterface, const StaggeredVectorField2D& velocityField, const Domain& domain, float timeStep) {
	using enum VectorComponent;
	
	int width = volumeFractionField.width();
	int height = volumeFractionField.height();
	float cellWidth = volumeFractionField.cellWidth();

	
	for (VectorComponent C : {X, Y}) {
		fluidInterface.reconstruct(volumeFractionField);
		computeFluxes(C, m_fluxes, fluidInterface, velocityField, volumeFractionField, timeStep);
		
		#pragma omp parallel for
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				float flux0 = m_fluxes.getEdgeValue(C, i, j);
				float flux1 = m_fluxes.getEdgeValue(C, i + (C == X ? 1 : 0), j + (C == Y ? 1 : 0));

				float cellVolume = cellWidth * cellWidth;
				float volumeFraction = volumeFractionField.getValue(i, j);
				float dilatationTerm = volumeFraction > 0.5f
					? Staggered::divergence(C, i, j, velocityField) * cellVolume
					: 0;
				float netFlux = flux0 - flux1;

				float newVolumeFraction = timeStep / cellVolume * (netFlux + dilatationTerm) + volumeFraction;
				volumeFractionField.setValue(i, j, newVolumeFraction);
			}
		}
	}
}


void Advection::computeFluxes(VectorComponent C, StaggeredGrid2D<float>& result, const FluidInterface2D& fluidInterface, const StaggeredVectorField2D& velocityField, const ScalarField2D& volumeFractionField, float timeStep) const {
	using enum VectorComponent;

	int width = velocityField.width();
	int height = velocityField.height();
	float cellWidth = velocityField.cellWidth();

	#pragma omp parallel for
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			float flux = fluidInterface.computeFluxAt(C, i, j, velocityField, volumeFractionField, cellWidth, timeStep);
			result.setEdgeValue(C, i, j, flux);
		}
	}
}
