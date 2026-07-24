#include "math/solvers/PressureSolver.h"


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
		if (i < iterationCount - 1)
			std::swap(auxField, result);
	}
}


//void PressureSolver::solveJacobi(ScalarField2D& result, float fixedDeltaTime, unsigned int iterationCount) {
//	int width = result.width();
//	int height = result.height();
//	for (unsigned int k = 0; k < iterationCount; k++) {
//		for (int i = 0; i < width; i++) {
//			for (int j = 0; j < height; j++) {
//				float newCellValue = solveCell(m_auxScalarField, fixedDeltaTime, i, j);
//				result.setValue(i, j, newCellValue);
//			}
//		}
//		if (k < iterationCount - 1)
//			std::swap(result, m_auxScalarField);
//	}
//}


//float PressureSolver::solveCell(ScalarField2D& prevPressureField, float fixedDeltaTime, int i, int j) {
//	float right = prevPressureField.getValue(i + 1, j);
//	float left = prevPressureField.getValue(i - 1, j);
//	float top = prevPressureField.getValue(i, j + 1);
//	float bottom = prevPressureField.getValue(i, j - 1);
//
//	float rightVel = m_velocityField.getEdgeX(i + 1, j);
//	float leftVel = m_velocityField.getEdgeX(i, j);
//	float topVel = m_velocityField.getEdgeY(i, j + 1);
//	float bottomVel = m_velocityField.getEdgeY(i, j);
//
//	float k = m_cellWidth * m_density / fixedDeltaTime;
//	float newPressure = (right + left + top + bottom);
//	newPressure -= k * (rightVel - leftVel + topVel - bottomVel);
//	newPressure /= 4;
//
//	return newPressure;
//}


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
