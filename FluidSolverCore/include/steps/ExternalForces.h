#pragma once

#include "math/dataStructures/StaggeredVectorField.h"
#include "math/dataStructures/Grid.h"

#include "domain/BoundaryData.h"
#include "domain/CellData.h"

namespace ExternalForces {
	void applyGravity(StaggeredVectorField2D& velocityField, const StaggeredGrid2D<BoundaryData>& boundaryData, const Grid2D<CellData>& cellData, float timeStep, float gravity = -9.8f);
}
