#include <omp.h>

#include "steps/ExternalForces.h"


namespace ExternalForces {
	void applyGravity(StaggeredVectorField2D& velocityField, const Domain& domain, const ScalarField2D& volumeFractionField, float timeStep, float gravity) {
		int width = velocityField.getValuesWidth(VectorComponent::Y);
		int height = velocityField.getValuesHeight(VectorComponent::Y);

		#pragma omp parallel for
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				if (volumeFractionField.getValue(i, j) == 0.0f && volumeFractionField.getValue(i, j - 1) == 0.0f) continue;
				if (domain.hasEdgePrescribedVelocity(VectorComponent::Y, i, j)) continue;
				float currentVel = velocityField.getEdgeValue(VectorComponent::Y, i, j);
				velocityField.setEdgeValue(VectorComponent::Y, i, j, currentVel + gravity * timeStep);
			}
		}
	}

	void applyGravity(StaggeredVectorField2D& velocityField, const Domain& domain, float timeStep, float gravity) {
		int width = velocityField.getValuesWidth(VectorComponent::Y);
		int height = velocityField.getValuesHeight(VectorComponent::Y);

		#pragma omp parallel for
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				if (not domain.isFluidEdge(VectorComponent::Y, i, j)) continue;
				if (domain.hasEdgePrescribedVelocity(VectorComponent::Y, i, j)) continue;
				float currentVel = velocityField.getEdgeValue(VectorComponent::Y, i, j);
				velocityField.setEdgeValue(VectorComponent::Y, i, j, currentVel + gravity * timeStep);
			}
		}
	}
}
