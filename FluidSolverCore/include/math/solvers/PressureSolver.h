#pragma once

#include "math/dataStructures/ScalarField.h"
#include "math/dataStructures/StaggeredVectorField.h"

#include "domain/Domain.h"

class PressureSolver {
public:
	PressureSolver(int gridWidth, int gridHeight, float cellWidth) :
		m_auxScalarField(gridWidth, gridHeight, cellWidth) {}

	void solveJacobi(ScalarField2D& result, StaggeredVectorField2D& velocityField, const Domain& domain, float density, float dt, unsigned int iterationCount);
	void solveJacobi(ScalarField2D& result, StaggeredVectorField2D& velocityField, const ScalarField2D& volumeFractionField, const Domain&, float density, float dt, unsigned int iterationCount);

private:
	float solveCell(int i ,int j, const ScalarField2D& prevPressureField, const StaggeredVectorField2D& velocityField, const Domain& domain, float density, float dt);
	float solveCell(int i, int j, const ScalarField2D& prevPressureField, const StaggeredVectorField2D& velocityField, const ScalarField2D& volumeFractionField, const Domain&, float density, float dt);
	
	ScalarField2D m_auxScalarField;
};
