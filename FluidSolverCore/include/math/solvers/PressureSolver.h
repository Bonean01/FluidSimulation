#pragma once

#include <cstdint>

#include "math/dataStructures/ScalarField.h"
#include "math/dataStructures/VectorField.h"
#include "math/dataStructures/MACGrid.h"

class PressureSolver {
public:
	PressureSolver(MACGrid2D& velocityField, Grid2D<uint8_t>& solidCellMap) :
		m_cellWidth(velocityField.cellWidth()),
		m_velocityField(velocityField),
		m_auxScalarField(velocityField.width(), velocityField.height(), m_cellWidth),
		m_solidCellMap(solidCellMap) {}

	void solveJacobi(ScalarField2D& result, float density, float dt, unsigned int iterationCount);
	float solveCell(int i ,int j, ScalarField2D& prevPressureField, float density, float dt);

private:
	float m_cellWidth;
	MACGrid2D& m_velocityField;
	ScalarField2D m_auxScalarField;
	Grid2D<uint8_t>& m_solidCellMap;
};
