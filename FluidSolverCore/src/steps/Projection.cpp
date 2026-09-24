#include "steps/Projection.h"

#include <omp.h>

#include "utils/profiling/ScopeProfiler.h"


void Projection::execute(StaggeredVectorField2D& velocityField, const ScalarField2D& pressureField, const Domain& domain, float density, float timeStep) {
	ScopeProfiler p{ "Projection" };
	using enum VectorComponent;

	projectComponent(X, velocityField, pressureField, domain, density, timeStep);
	projectComponent(Y, velocityField, pressureField, domain, density, timeStep);	
}


void Projection::projectComponent(const VectorComponent& C, StaggeredVectorField2D& velocityField, const ScalarField2D& pressureField, const Domain& domain, float density, float timeStep) {
		int width = velocityField.getValuesWidth(C);
		int height = velocityField.getValuesHeight(C);
		
		#pragma omp parallel for
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				if (domain.hasEdgePrescribedVelocity(C, i, j)) continue;

				float gradient = Staggered::gradient(C, i, j, pressureField);
				float currentVel = velocityField.getEdgeValue(C, i, j);
				float resultingVel = currentVel - gradient * timeStep / density;
				velocityField.setEdgeValue(C, i, j, resultingVel);
			}
		}
	}
