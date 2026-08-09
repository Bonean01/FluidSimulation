#include "steps/Projection.h"


void Projection::execute(StaggeredVectorField2D& velocityField, const ScalarField2D& pressureField, const StaggeredGrid2D<BoundaryData>& boundaryData, float density, float timeStep) {
	using enum VectorComponent;

	projectComponent(X, velocityField, pressureField, boundaryData, density, timeStep);
	projectComponent(Y, velocityField, pressureField, boundaryData, density, timeStep);	
}


void Projection::projectComponent(const VectorComponent& C, StaggeredVectorField2D& velocityField, const ScalarField2D& pressureField, const StaggeredGrid2D<BoundaryData>& boundaryData, float density, float timeStep) {
		int width = velocityField.getValuesWidth(C);
		int height = velocityField.getValuesHeight(C);

		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				if (BoundaryUtils::hasBoundaryPrescribedVelocity(C, i, j, boundaryData)) continue;

				float gradient = Staggered::gradient(C, i, j, pressureField);
				float currentVel = velocityField.getEdgeValue(C, i, j);
				float resultingVel = currentVel - gradient * timeStep / density;
				velocityField.setEdgeValue(C, i, j, resultingVel);
			}
		}
	}
