#pragma once

#include "math/dataStructures/ScalarField.h"
#include "math/dataStructures/VectorField.h"
#include "math/solvers/JacobiIterationSolver.h"

class PressureSolver {
public:
	PressureSolver(VectorField2D& velocityField, Grid2D<uint8_t>& solidCellMap) : 
		m_velocityField(velocityField),
		m_solidCellMap(solidCellMap),
		m_velocityDivergence(velocityField.width(), velocityField.height(), velocityField.cellWidth()),
		m_auxScalarField(velocityField.width(), velocityField.height(), velocityField.cellWidth()) {}

	void solve(ScalarField2D& result, unsigned int iterationCount);
	float solveCell(ScalarField2D& prevPressureField, int i, int j);
	float calculateError();

private:
	JacobiIterationSolver m_jacobiSolver{};
	VectorField2D& m_velocityField;
	ScalarField2D m_velocityDivergence, m_auxScalarField;
	Grid2D<uint8_t>& m_solidCellMap;
};
