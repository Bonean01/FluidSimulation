#pragma once

#include "math/dataStructures/ScalarField.h"
#include "math/dataStructures/StaggeredVectorField.h"

#include "domain/CellData.h"

class PressureSolver {
public:
	PressureSolver(int gridWidth, int gridHeight, float cellWidth) :
		m_auxScalarField(gridWidth, gridHeight, cellWidth) {}

	void solveJacobi(ScalarField2D& result, StaggeredVectorField2D& velocityField, const Grid2D<CellData>& cellData, float density, float dt, unsigned int iterationCount);
	float solveCell(int i ,int j, ScalarField2D& prevPressureField, StaggeredVectorField2D& velocityField, const Grid2D<CellData>& cellData, float density, float dt);

private:
	ScalarField2D m_auxScalarField;
};
