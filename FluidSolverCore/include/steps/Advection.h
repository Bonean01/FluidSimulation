#pragma once

#include "math/dataStructures/StaggeredVectorField.h"
#include "math/dataStructures/Grid.h"
#include "math/dataStructures/ScalarField.h"
#include "math/dataStructures/StaggeredGrid.h"

#include "domain/BoundaryData.h"
#include "domain/MarkerParticle.h"
#include "domain/Domain.h"

#include "domain/PLIC/FluidInterface.h"

class Advection {
public:
	Advection(int gridWidth, int gridHeight, float cellWidth) :
		m_auxStaggeredVectorField(gridWidth, gridHeight, cellWidth),
		m_auxScalarField(gridWidth, gridHeight, cellWidth),
		m_fluxes(gridWidth, gridHeight, cellWidth) {}

	// self-advection
	void execute(StaggeredVectorField2D& velocityField, const Domain& domain, float timeStep);
	void execute(ScalarField2D& field, const StaggeredVectorField2D& velocityField, const Domain& domain, float timeStep);
	void execute(std::vector<MarkerParticle>& markerParticles, const StaggeredVectorField2D& velocityField, const Domain& domain, float timeStep);
	void execute(ScalarField2D& volumeFractionField, FluidInterface2D& fluidInterface, const StaggeredVectorField2D& velocityField, const Domain& domain, float timeStep);


	void computeFluxes(VectorComponent C, StaggeredGrid2D<float>& result, const FluidInterface2D& fluidInterface, const StaggeredVectorField2D& velocityField, const ScalarField2D& volumeFractionField, float timeStep) const;
private:
	StaggeredVectorField2D m_auxStaggeredVectorField;
	StaggeredGrid2D<float> m_fluxes;
	ScalarField2D m_auxScalarField;

	void advectComponent(const VectorComponent& C, StaggeredVectorField2D& velocityField, const Domain& domain, float timeStep);
};
