#include <omp.h>

#include "steps/ExternalForces.h"


namespace ExternalForces {
	void applyGravity(StaggeredVectorField2D& velocityField, const Domain& domain, float timeStep, float gravity) {
		int width = velocityField.getValuesWidth(VectorComponent::Y);
		int height = velocityField.getValuesHeight(VectorComponent::Y);

		#pragma omp parallel for
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				if (domain.getCell(i, j).cellType != CellType::Fluid && domain.getCell(i, j - 1).cellType != CellType::Fluid) continue;
				if (domain.hasEdgePrescribedVelocity(VectorComponent::Y, i, j)) continue;
				float currentVel = velocityField.getEdgeValue(VectorComponent::Y, i, j);
				velocityField.setEdgeValue(VectorComponent::Y, i, j, currentVel + gravity * timeStep);
			}
		}
	}
}
