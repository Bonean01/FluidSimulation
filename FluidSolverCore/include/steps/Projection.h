#pragma once

#include "math/dataStructures/StaggeredVectorField.h"
#include "math/dataStructures/ScalarField.h"
#include "math/dataStructures/Grid.h"
#include "domain/CellData.h"

class Projection {
public:
	// Subtract the divergent component of the velocity field from it
	void execute(StaggeredVectorField2D& velocityField, const ScalarField2D& pressureField, const Grid2D<CellData>& cellData, float density, float timeStep);
};
