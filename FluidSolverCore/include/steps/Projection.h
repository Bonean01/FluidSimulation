#pragma once

#include "math/dataStructures/StaggeredVectorField.h"
#include "math/dataStructures/ScalarField.h"
#include "math/dataStructures/Grid.h"
#include "domain/CellData.h"
#include "domain/BoundaryUtils.h"

class Projection {
public:
	// Subtract the divergent component of the velocity field from it
	void execute(StaggeredVectorField2D& velocityField, const ScalarField2D& pressureField, const Grid2D<CellData>& cellData, float density, float timeStep);

private:
	template<VectorComponent C>
	void projectComponent(StaggeredVectorField2D& velocityField, const ScalarField2D& pressureField, const Grid2D<CellData>& cellData, float density, float timeStep) {
		int width = velocityField.getValuesWidth();
		int height = velocityField.getValuesHeight();

		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				if (BoundaryUtils::hasBoundaryPrescribedVelocity(C, cellData, i, j)) continue;

				float gradient = Staggered::gradient<C>(i, j, pressureField);
				float currentVel = velocityField.getEdgeValue<C>(i, j);
				float resultingVel = currentVel - gradient * timeStep / density;
				velocityField.setEdgeValue<C>(i, j, resultingVel);
			}
		}
	}
};
