#include "math/solvers/PressureSolver.h"


void PressureSolver::solveJacobi(ScalarField2D& result, float density, float dt, unsigned int iterationCount) {
	int width = result.width();
	int height = result.height();
	for (unsigned int k = 0; k < iterationCount; k++) {
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				float newCellValue = solveCell(i, j, m_auxScalarField, density, dt);
				result.setValue(i, j, newCellValue);
			}
		}
		if (k < iterationCount - 1)
			std::swap(result, m_auxScalarField);
	}
}


float PressureSolver::solveCell(int i, int j, ScalarField2D& pressureField, float density, float dt) {
	bool isSolid = m_solidCellMap.getValue(i, j) == 1;

	uint8_t rightFluid = 1 - m_solidCellMap.getValue(i + 1, j);
	uint8_t leftFluid = 1 - m_solidCellMap.getValue(i - 1, j);
	uint8_t topFluid = 1 - m_solidCellMap.getValue(i, j + 1);
	uint8_t bottomFluid = 1 - m_solidCellMap.getValue(i, j - 1);
	
	int totalFluidCells = rightFluid + leftFluid + topFluid + bottomFluid;

	if (isSolid || totalFluidCells == 0) return 0.0f;

	float rightPres = pressureField.getValue(i + 1, j) * rightFluid;
	float leftPres = pressureField.getValue(i - 1, j) * leftFluid;
	float topPres = pressureField.getValue(i, j + 1) * topFluid;
	float bottomPres = pressureField.getValue(i, j - 1) * bottomFluid;

	float rightVel = m_velocityField.getEdgeX(i + 1, j) * rightFluid;
	float leftVel = m_velocityField.getEdgeX(i, j) * leftFluid;
	float topVel = m_velocityField.getEdgeY(i, j + 1) * topFluid;
	float bottomVel = m_velocityField.getEdgeY(i, j) * bottomFluid;

	float pressureSum = rightPres + leftPres + topPres + bottomPres;
	float k = m_cellWidth * density / dt;

	float newPressure = pressureSum - k * (rightVel - leftVel + topVel - bottomVel);
	newPressure /= totalFluidCells;

	return newPressure;
}
