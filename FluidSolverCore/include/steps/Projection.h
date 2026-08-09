#pragma once

#include "math/dataStructures/StaggeredVectorField.h"
#include "math/dataStructures/ScalarField.h"
#include "math/dataStructures/Grid.h"
#include "math/operators/Staggered.h"
#include "domain/CellData.h"
#include "domain/BoundaryUtils.h"

class Projection {
public:
	// Subtract the divergent component of the velocity field from it
	void execute(StaggeredVectorField2D& velocityField, const ScalarField2D& pressureField, const StaggeredGrid2D<BoundaryData>&, float density, float timeStep);

private:
	void projectComponent(const VectorComponent& C, StaggeredVectorField2D& velocityField, const ScalarField2D& pressureField, const StaggeredGrid2D<BoundaryData>& boundaryData, float density, float timeStep);
};
