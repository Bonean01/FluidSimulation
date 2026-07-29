#include "steps/Projection.h"

#include "math/operators/Staggered.h"


void Projection::execute(MACGrid2D& velocityField, const ScalarField2D& pressureField, const Grid2D<uint8_t>& solidCellMap, float density, float timeStep) {
	int width = velocityField.width();
	int height = velocityField.height();

	// === horizontal ===
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width + 1; i++) {
			if (solidCellMap.getValue(i, j) || solidCellMap.getValue(i - 1, j)) { velocityField.setEdgeX(i, j, 0.0f); continue; }
			float gradientX = Staggered::gradientX(i, j, pressureField);
			float currentVel = velocityField.getEdgeX(i, j);
			float resultingVel = currentVel - gradientX * timeStep / density;
			velocityField.setEdgeX(i, j, resultingVel);
		}
	}
	// === vertical ===
	for (int j = 0; j < height + 1; j++) {
		for (int i = 0; i < width; i++) {
			if (solidCellMap.getValue(i, j) || solidCellMap.getValue(i, j - 1)) { velocityField.setEdgeY(i, j, 0.0f); continue; }
			float gradientY = Staggered::gradientY(i, j, pressureField);
			float currentVel = velocityField.getEdgeY(i, j);
			float resultingVel = currentVel - gradientY * timeStep / density;
			velocityField.setEdgeY(i, j, resultingVel);
		}
	}
}
