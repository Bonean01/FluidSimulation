#include <omp.h>

#include "steps/ExternalForces.h"
#include "domain/DomainUtils.h"


namespace ExternalForces {
	void applyGravity(StaggeredVectorField2D& velocityField, const StaggeredGrid2D<BoundaryData>& boundaryData, const Grid2D<CellData>& cellData, float timeStep, float gravity) {
		int width = velocityField.getValuesWidth(VectorComponent::Y);
		int height = velocityField.getValuesHeight(VectorComponent::Y);

		#pragma omp parallel for
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				if (cellData.getValue(i, j).cellType != CellType::Fluid && cellData.getValue(i, j - 1).cellType != CellType::Fluid) continue;
				BoundaryData boundary = boundaryData.getEdgeValue(VectorComponent::Y, i, j);
				if (DomainUtils::hasBoundaryPrescribedVelocity(boundary)) continue;
				float currentVel = velocityField.getEdgeValue(VectorComponent::Y, i, j);
				velocityField.setEdgeValue(VectorComponent::Y, i, j, currentVel + gravity * timeStep);
			}
		}
	}
}
