#pragma once

#include "math/dataStructures/MACGrid.h"
#include "math/dataStructures/Grid.h"
#include "math/dataStructures/ScalarField.h"


class Advection {
public:
	Advection(int gridWidth, int gridHeight, float cellWidth) :
		m_auxMacGrid(gridWidth, gridHeight, cellWidth),
		m_auxScalarField(gridWidth, gridHeight, cellWidth) {}

	// self-advection
	void execute(MACGrid2D& velocityField, const Grid2D<uint8_t>& solidCellMap, float timeStep);
	void execute(ScalarField2D& field, const MACGrid2D& velocityField, const Grid2D<uint8_t>& solidCellMap, float timeStep);


private:
	MACGrid2D m_auxMacGrid;
	ScalarField2D m_auxScalarField;
};
