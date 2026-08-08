#include "steps/Projection.h"

#include "math/operators/Staggered.h"
#include "domain/BoundaryConditions.h"

// TODO: Refactor the boundary checking so that cells next to cells with prescribed pressure get properly projected
void Projection::execute(MACGrid2D& velocityField, const ScalarField2D& pressureField, const Grid2D<CellData>& cellData, float density, float timeStep) {
	int width = velocityField.width();
	int height = velocityField.height();

	// === horizontal ===
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width + 1; i++) {
			if (BoundaryUtils::hasBoundaryPrescribedVelocity(VectorComponent::X, cellData, i, j)) continue;

			float gradientX = Staggered::gradientX(i, j, pressureField);
			float currentVel = velocityField.getEdgeX(i, j);
			float resultingVel = currentVel - gradientX * timeStep / density;
			velocityField.setEdgeX(i, j, resultingVel);
		}
	}
	// === vertical ===
	for (int j = 0; j < height + 1; j++) {
		for (int i = 0; i < width; i++) {
			if (BoundaryUtils::hasBoundaryPrescribedVelocity(VectorComponent::Y, cellData, i, j)) continue;

			float gradientY = Staggered::gradientY(i, j, pressureField);
			float currentVel = velocityField.getEdgeY(i, j);
			float resultingVel = currentVel - gradientY * timeStep / density;
			velocityField.setEdgeY(i, j, resultingVel);
		}
	}
}
