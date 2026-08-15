#pragma once

#include "math/dataStructures/StaggeredVectorField.h"
#include "math/dataStructures/ScalarField.h"
#include "BoundaryData.h"
#include "math/dataStructures/Grid.h"
#include "domain/BoundaryCondition.h"


namespace BoundaryUtils {
	void applyVelocityBoundaryConditions(StaggeredVectorField2D& velocityField, const StaggeredGrid2D<BoundaryData>& boundaryData);
	void applyVelocityBCsToComponent(const VectorComponent& C, StaggeredVectorField2D& velocityField, const StaggeredGrid2D<BoundaryData>& boundaryData);
	void applyVelocityBCsToEdge(const VectorComponent& C, int i, int j, StaggeredVectorField2D& velocityField, const BoundaryData& boundaryData);
	bool hasPrescribedVelocity(const BoundaryData& boundaryData);
}
