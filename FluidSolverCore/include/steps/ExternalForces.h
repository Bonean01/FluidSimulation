#pragma once

#include "math/dataStructures/StaggeredVectorField.h"
#include "math/dataStructures/Grid.h"

#include "domain/BoundaryData.h"

namespace ExternalForces {
	void applyGravity(StaggeredVectorField2D& velocityField, const StaggeredGrid2D<BoundaryData>& boundaryData, float timeStep, float gravity = -9.8f);
}
