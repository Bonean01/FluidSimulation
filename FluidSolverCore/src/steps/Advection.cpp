#include "steps/Advection.h"

#include "domain/BoundaryConditions.h"


void Advection::execute(MACGrid2D& velocityField, const Grid2D<CellData>& cellData, float timeStep) {
	int width = velocityField.width();
	int height = velocityField.height();
	if (m_auxMacGrid.width() != width || m_auxMacGrid.height() != height) return;

	// === horizontal ===
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width + 1; i++) {
			CellData rightCellData = cellData.getValue(i, j);
			CellData leftCellData = cellData.getValue(i - 1, j);

			if (rightCellData.boundaryType != BoundaryType::None || leftCellData.boundaryType != BoundaryType::None) {
				CellData cellData = rightCellData.boundaryType != BoundaryType::None ? rightCellData : leftCellData;
				BoundaryConditions::applyVelocityX(i, j, m_auxMacGrid, cellData);
				continue;
			}

			Vec2f position = { (float)i - 0.5f, (float)j };
			Vec2f currentVel = velocityField.sampleBilinear(position);
			Vec2f newValue = velocityField.sampleBilinear(position - currentVel * timeStep);
			m_auxMacGrid.setEdgeX(i, j, newValue.x);
		}
	}
	// === vertical ===
	for (int j = 0; j < height + 1; j++) {
		for (int i = 0; i < width; i++) {
			CellData topCellData = cellData.getValue(i, j);
			CellData bottomCellData = cellData.getValue(i, j - 1);

			if (topCellData.boundaryType != BoundaryType::None || bottomCellData.boundaryType != BoundaryType::None) {
				CellData cellData = topCellData.boundaryType != BoundaryType::None ? topCellData : bottomCellData;
				BoundaryConditions::applyVelocityY(i, j, m_auxMacGrid, cellData);
				continue;
			}
			
			Vec2f position = { (float)i, (float)j - 0.5f };
			Vec2f currentVel = velocityField.sampleBilinear(position);
			Vec2f newValue = velocityField.sampleBilinear(position - currentVel * timeStep);
			m_auxMacGrid.setEdgeY(i, j, newValue.y);
		}
	}
	std::swap(velocityField, m_auxMacGrid);
}


void Advection::execute(ScalarField2D& field, const MACGrid2D& velocityField, const Grid2D<CellData>& cellData, float timeStep) {
	int width = field.width();
	int height = field.height();
	if (m_auxScalarField.width() != width || m_auxScalarField.height() != height) return;

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			if (cellData.getValue(i, j).cellType == CellType::Solid) { m_auxScalarField.setValue(i, j, 0.0f); continue; }
			Vec2f position = { (float)i, (float)j };
			Vec2f currentVel = velocityField.sampleBilinear(position);
			float newValue = field.sampleBilinear(position - currentVel * timeStep);
			m_auxScalarField.setValue(i, j, newValue);
		}
	}
	std::swap(field, m_auxScalarField);
}
