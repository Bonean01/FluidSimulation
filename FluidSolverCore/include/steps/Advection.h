#pragma once

#include "math/dataStructures/StaggeredVectorField.h"
#include "math/dataStructures/Grid.h"
#include "math/dataStructures/ScalarField.h"

#include "domain/CellData.h"
#include "domain/BoundaryData.h"
#include "domain/MarkerParticle.h"
#include "domain/DomainUtils.h"

class Advection {
public:
	Advection(int gridWidth, int gridHeight, float cellWidth) :
		m_auxStaggeredVectorField(gridWidth, gridHeight, cellWidth),
		m_auxScalarField(gridWidth, gridHeight, cellWidth) {}

	// self-advection
	void execute(StaggeredVectorField2D& velocityField, const Domain& domain, float timeStep);
	void execute(ScalarField2D& field, const StaggeredVectorField2D& velocityField, const Domain& domain, float timeStep);
	void execute(Domain& domain, const StaggeredVectorField2D& velocityField, float timeStep);


private:
	StaggeredVectorField2D m_auxStaggeredVectorField;
	ScalarField2D m_auxScalarField;

	void advectComponent(const VectorComponent& C, StaggeredVectorField2D& velocityField, const Domain& domain, float timeStep);
};
