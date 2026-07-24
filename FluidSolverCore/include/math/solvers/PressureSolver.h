#pragma once

#include "math/dataStructures/ScalarField.h"
#include "math/dataStructures/VectorField.h"
#include "math/dataStructures/MACGrid.h"
#include "math/solvers/JacobiIterationSolver.h"

class PressureSolver {
public:
	PressureSolver(float density, float kinematicViscosity, MACGrid2D& velocityField, Grid2D<uint8_t>& solidCellMap) : 
		m_velocityField(velocityField),
		m_solidCellMap(solidCellMap),
		m_density(density),
		m_kinematicViscosity(kinematicViscosity),
		m_cellWidth(velocityField.cellWidth()),
		m_velocityDivergence(velocityField.width(), velocityField.height(), m_cellWidth),
		m_auxScalarField(velocityField.width(), velocityField.height(), m_cellWidth) {}

	void solve(ScalarField2D& result, unsigned int iterationCount);
	void solveJacobi(ScalarField2D& result, float fixedDeltaTime, unsigned int iterationCount);
	float solveCell(ScalarField2D& prevPressureField, float fixedDeltaTime, int i, int j);
	float calculateError();

private:
	JacobiIterationSolver m_jacobiSolver{};
	MACGrid2D& m_velocityField;
	ScalarField2D m_velocityDivergence, m_auxScalarField;
	Grid2D<uint8_t>& m_solidCellMap;
	float m_density, m_kinematicViscosity, m_cellWidth;
};
