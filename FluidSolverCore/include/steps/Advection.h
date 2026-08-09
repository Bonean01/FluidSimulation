#pragma once

#include "math/dataStructures/StaggeredVectorField.h"
#include "math/dataStructures/Grid.h"
#include "math/dataStructures/ScalarField.h"

#include "domain/CellData.h"

class Advection {
public:
	Advection(int gridWidth, int gridHeight, float cellWidth) :
		m_auxStaggeredVectorField(gridWidth, gridHeight, cellWidth),
		m_auxScalarField(gridWidth, gridHeight, cellWidth) {}

	// self-advection
	void execute(StaggeredVectorField2D& velocityField, const Grid2D<CellData>& cellData, float timeStep);
	void execute(ScalarField2D& field, const StaggeredVectorField2D& velocityField, const Grid2D<CellData>& cellData, float timeStep);

private:
	StaggeredVectorField2D m_auxStaggeredVectorField;
	ScalarField2D m_auxScalarField;
};
