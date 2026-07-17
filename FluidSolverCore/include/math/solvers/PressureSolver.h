#pragma once

#include "math/dataStructures/ScalarField.h"
#include "math/dataStructures/VectorField.h"
#include "math/solvers/JacobiIterationSolver.h"

class PressureSolver {
public:
	void solve(ScalarField2D& result, ScalarField2D& auxField, unsigned int iterationCount);
	PressureSolver(VectorField2D& velocityField) : 
		m_velocityField(velocityField),
		m_velocityDivergence(velocityField.width(), velocityField.height(), velocityField.cellWidth()) {}

private:
	JacobiIterationSolver m_jacobiSolver{};
	VectorField2D& m_velocityField;
	ScalarField2D m_velocityDivergence;
};
