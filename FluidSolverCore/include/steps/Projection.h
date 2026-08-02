#pragma once

#include "math/dataStructures/MACGrid.h"
#include "math/dataStructures/ScalarField.h"
#include "math/dataStructures/Grid.h"

class Projection {
public:
	// Subtract the divergent component of the velocity field from it
	void execute(MACGrid2D& velocityField, const ScalarField2D& pressureField, float density, float timeStep);
};
