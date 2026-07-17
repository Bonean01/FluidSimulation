#include "math/solvers/PressureSolver.h"


void PressureSolver::solve(ScalarField2D& result, ScalarField2D& auxField, unsigned int iterationCount) {
	int width = result.width();
	int height = result.height();

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			float divergence = m_velocityField.divergence(i, j);
			m_velocityDivergence.setValue(i, j, divergence);
		}
	}

	float dx = m_velocityDivergence.cellWidth();
	float alpha = -dx * dx;
	float beta = 4;
	for (unsigned int i = 0; i < iterationCount; i++) {
		m_jacobiSolver.solve(auxField, result, m_velocityDivergence, alpha, beta);
		std::swap(auxField, result);
	}
}
