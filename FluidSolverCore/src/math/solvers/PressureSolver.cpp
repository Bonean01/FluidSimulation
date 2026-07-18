#include "math/solvers/PressureSolver.h"


void PressureSolver::solve(ScalarField2D& result, unsigned int iterationCount) {
	int width = result.width();
	int height = result.height();
	ScalarField2D& auxField = m_auxScalarField;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			float divergence = m_velocityField.divergence(i, j);
			m_velocityDivergence.setValue(i, j, divergence);
		}
	}

	float dx = m_velocityDivergence.cellWidth();
	float cellSize = dx * dx;
	float beta = 4;
	float alpha = -cellSize;
	for (unsigned int i = 0; i < iterationCount; i++) {
		int adyacentFluidCellCount = 0; // depends on the current cell
		m_jacobiSolver.solve(auxField, result, m_velocityDivergence, alpha, beta);
		std::swap(auxField, result);
	}
}


float PressureSolver::calculateError() {
	float error = 0;
	int width = m_velocityField.width();
	int height = m_velocityField.height();

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			error += m_velocityField.divergence(i, j);
		}
	}
	return error;
}
