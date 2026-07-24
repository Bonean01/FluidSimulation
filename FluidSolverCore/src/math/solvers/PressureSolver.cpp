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
		if (i < iterationCount - 1)
			std::swap(auxField, result);
	}
}


void PressureSolver::solveJacobi(ScalarField2D& result, float fixedDeltaTime, unsigned int iterationCount) {
	int width = result.width();
	int height = result.height();
	for (unsigned int k = 0; k < iterationCount; k++) {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				float newCellValue = solveCell(m_auxScalarField, fixedDeltaTime, i, j);
				result.setValue(i, j, newCellValue);
			}
		}
		if (k < iterationCount - 1)
			std::swap(result, m_auxScalarField);
	}
}


float PressureSolver::solveCell(ScalarField2D& prevPressureField, float fixedDeltaTime, int i, int j) {
	uint8_t rightFluid = 1 - m_solidCellMap.getValue(i + 1, j);
	uint8_t leftFluid = 1 - m_solidCellMap.getValue(i - 1, j);
	uint8_t topFluid = 1 - m_solidCellMap.getValue(i, j + 1);
	uint8_t bottomFluid = 1 - m_solidCellMap.getValue(i, j - 1);
	bool isSolid = m_solidCellMap.getValue(i, j) == 1;
	
	int totalFluidCells = rightFluid + leftFluid + topFluid + bottomFluid;

	if (isSolid || totalFluidCells == 0) return 0.0f;

	float rightPres = prevPressureField.getValue(i + 1, j) * rightFluid;
	float leftPres = prevPressureField.getValue(i - 1, j) * leftFluid;
	float topPres = prevPressureField.getValue(i, j + 1) * topFluid;
	float bottomPres = prevPressureField.getValue(i, j - 1) * bottomFluid;

	float rightVel = m_velocityField.getEdgeX(i + 1, j) * rightFluid;
	float leftVel = m_velocityField.getEdgeX(i, j) * leftFluid;
	float topVel = m_velocityField.getEdgeY(i, j + 1) * topFluid;
	float bottomVel = m_velocityField.getEdgeY(i, j) * bottomFluid;


	float k = (m_cellWidth * m_density) / fixedDeltaTime;
	float newPressure = (rightPres + leftPres + topPres + bottomPres);
	newPressure -= k * (rightVel - leftVel + topVel - bottomVel);
	newPressure /= totalFluidCells;

	//if (i == 2 && j == 2) {
	//	std::cout << "isSolid: " << isSolid << std::endl;
	//	std::cout << "totalFluidCells: " << totalFluidCells << std::endl;
	//	std::cout << "rPres: " << rightPres << std::endl;
	//	std::cout << "lPres: " << leftPres << std::endl;
	//	std::cout << "tPres: " << topPres << std::endl;
	//	std::cout << "bPres: " << bottomPres << std::endl;
	//	std::cout << "rVel: " << rightVel << std::endl;
	//	std::cout << "lVel: " << leftVel << std::endl;
	//	std::cout << "tVel: " << topVel << std::endl;
	//	std::cout << "bVel: " << bottomVel << std::endl;
	//	std::cout << "cellWidth: " << m_cellWidth << std::endl;
	//	std::cout << "density: " << m_density << std::endl;
	//	std::cout << "deltaTime: " << fixedDeltaTime << std::endl;
	//	std::cout << "newPress: " << newPressure << std::endl << std::endl;
	//}

	return newPressure;
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
