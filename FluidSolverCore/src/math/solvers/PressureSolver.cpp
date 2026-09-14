#include "math/solvers/PressureSolver.h"

#include <omp.h>

#include "utils/profiling/ScopeProfiler.h"


void PressureSolver::solveJacobi(ScalarField2D& result, StaggeredVectorField2D& velocityField, const Grid2D<CellData>& cellData, float density, float timeStep, unsigned int iterationCount) {
	ScopeProfiler p{ "Pressure Solve" };

	int width = result.width();
	int height = result.height();
	for (unsigned int k = 0; k < iterationCount; k++) {
		#pragma omp parallel for
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				float newCellValue = solveCell(i, j, m_auxScalarField, velocityField, cellData, density, timeStep);
				result.setValue(i, j, newCellValue);
			}
		}
		if (k < iterationCount - 1)
			std::swap(result, m_auxScalarField);
	}
}


float PressureSolver::solveCell(int i, int j, ScalarField2D& pressureField, StaggeredVectorField2D& velocityField, const Grid2D<CellData>& cellData, float density, float timeStep) {
	using enum VectorComponent;
	
	bool isSolid = cellData.getValue(i, j).cellType == CellType::Solid;
	bool isVoid = cellData.getValue(i, j).cellType == CellType::Void;

	bool rightFluid = cellData.getValue(i + 1, j).cellType == CellType::Fluid;
	bool leftFluid = cellData.getValue(i - 1, j).cellType == CellType::Fluid;
	bool topFluid = cellData.getValue(i, j + 1).cellType == CellType::Fluid;
	bool bottomFluid = cellData.getValue(i, j - 1).cellType == CellType::Fluid;

	bool rightSolid = cellData.getValue(i + 1, j).cellType == CellType::Solid;
	bool leftSolid = cellData.getValue(i - 1, j).cellType == CellType::Solid;
	bool topSolid = cellData.getValue(i, j + 1).cellType == CellType::Solid;
	bool bottomSolid = cellData.getValue(i, j - 1).cellType == CellType::Solid;

	int totalSolidCells = rightSolid + leftSolid + topSolid + bottomSolid;
	if (isSolid || isVoid || totalSolidCells == 4) return 0.0f;

	float rightPres = pressureField.getValue(i + 1, j);
	float leftPres = pressureField.getValue(i - 1, j);
	float topPres = pressureField.getValue(i, j + 1);
	float bottomPres = pressureField.getValue(i, j - 1);
	
	float rightVel = velocityField.getEdgeValue(X, i + 1, j);
	float leftVel = velocityField.getEdgeValue(X, i, j);
	float topVel = velocityField.getEdgeValue(Y, i, j + 1);
	float bottomVel = velocityField.getEdgeValue(Y, i, j);

	// Don't include pressure's that come from solid cells
	// (theoretically they have 1 pressure value per face)
	float pressureSum = 0;
	if (rightFluid) { pressureSum += rightPres; }
	if (leftFluid) { pressureSum += leftPres; }
	if (topFluid) { pressureSum += topPres; }
	if (bottomFluid) { pressureSum += bottomPres; }

	float cellWidth = velocityField.cellWidth();
	float k = cellWidth * density / timeStep;

	float newPressure = pressureSum - k * (rightVel - leftVel + topVel - bottomVel);
	newPressure /= 4 - totalSolidCells;

	return newPressure;
}
