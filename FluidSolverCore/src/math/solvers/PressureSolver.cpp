#include "math/solvers/PressureSolver.h"

#include <iostream>


void PressureSolver::solve(ScalarField2D& result, unsigned int iterationCount) {
	int width = result.width();
	int height = result.height();
	ScalarField2D& auxField = m_auxScalarField;

	m_velocityField.divergence(m_velocityDivergence);

	float dx = m_velocityDivergence.cellWidth();
	float cellSize = dx * dx;
	float beta = 4;
	float alpha = -cellSize;
	for (unsigned int i = 0; i < iterationCount; i++) {
		m_jacobiSolver.solve(auxField, result, m_velocityDivergence, alpha, beta);
		std::swap(auxField, result);
	}
}


float PressureSolver::solveCell(ScalarField2D& prevPressureField, int i, int j) {
	if (m_solidCellMap.getValue(i, j) == 1) return 0.0f;
	int right = m_solidCellMap.getValue(i + 1, j);
	int left = m_solidCellMap.getValue(i - 1, j);
	int top = m_solidCellMap.getValue(i, j + 1);
	int bottom = m_solidCellMap.getValue(i, j - 1);
	int solidCells = right + left + top + bottom;

	float velocityDivergence = m_velocityField.divergence(i, j);

	float dx = m_velocityField.cellWidth();
	float cellSize = dx * dx;

	int beta = 4 - solidCells;
	if (beta == 0) beta = 1;
	float alpha = -cellSize;
	float res = m_jacobiSolver.solveCell(i, j, prevPressureField, velocityDivergence, alpha, (float)beta);
	return res;
}


float PressureSolver::calculateError() {
	float error = 0;
	int width = m_velocityField.width();
	int height = m_velocityField.height();

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (m_solidCellMap.getValue(i, j) == 1) continue;
			error += m_velocityField.divergence(i, j);
		}
	}
	return error;
}
