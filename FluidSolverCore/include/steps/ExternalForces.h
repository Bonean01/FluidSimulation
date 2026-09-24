#pragma once

#include "math/dataStructures/StaggeredVectorField.h"
#include "math/dataStructures/Grid.h"

#include "domain/Domain.h"

namespace ExternalForces {
	void applyGravity(StaggeredVectorField2D& velocityField, const Domain& domain, float timeStep, float gravity = -9.8f);
}
