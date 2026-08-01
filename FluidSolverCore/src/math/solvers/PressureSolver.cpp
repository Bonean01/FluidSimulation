#include "math/solvers/PressureSolver.h"


void PressureSolver::solveJacobi(ScalarField2D& result, float density, float timeStep, unsigned int iterationCount) {
	int width = result.width();
	int height = result.height();
	for (unsigned int k = 0; k < iterationCount; k++) {
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				float newCellValue = solveCell(i, j, m_auxScalarField, density, timeStep);
				result.setValue(i, j, newCellValue);
			}
		}
		if (k < iterationCount - 1)
			std::swap(result, m_auxScalarField);
	}
}


float PressureSolver::solveCell(int i, int j, ScalarField2D& pressureField, float density, float timeStep) {
	bool isSolid = m_solidCellMap.getValue(i, j);

	uint8_t rightFluid = 1 - m_solidCellMap.getValue(i + 1, j);
	uint8_t leftFluid = 1 - m_solidCellMap.getValue(i - 1, j);
	uint8_t topFluid = 1 - m_solidCellMap.getValue(i, j + 1);
	uint8_t bottomFluid = 1 - m_solidCellMap.getValue(i, j - 1);
	
	int totalFluidCells = rightFluid + leftFluid + topFluid + bottomFluid;

	if (isSolid || totalFluidCells == 0) return 0.0f;

	float rightPres = pressureField.getValue(i + 1, j);
	float leftPres = pressureField.getValue(i - 1, j);
	float topPres = pressureField.getValue(i, j + 1);
	float bottomPres = pressureField.getValue(i, j - 1);
	
	float rightVel = m_velocityField.getEdgeX(i + 1, j);
	float leftVel = m_velocityField.getEdgeX(i, j);
	float topVel = m_velocityField.getEdgeY(i, j + 1);
	float bottomVel = m_velocityField.getEdgeY(i, j);

	// Don't include pressure's that come from solid cells
	// (theoretically they have 1 pressure value per face)
	float pressureSum = 0;
	if (rightFluid) { pressureSum += rightPres; }
	if (leftFluid) { pressureSum += leftPres; }
	if (topFluid) { pressureSum += topPres; }
	if (bottomFluid) { pressureSum += bottomPres; }

	float k = m_cellWidth * density / timeStep;

	float newPressure = pressureSum - k * (rightVel - leftVel + topVel - bottomVel);
	newPressure /= totalFluidCells;

	return newPressure;
}
