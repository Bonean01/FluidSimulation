#pragma once

#include "math/dataStructures/StaggeredVectorField.h"
#include "math/dataStructures/Grid.h"

#include "domain/BoundaryData.h"
#include "domain/CellData.h"
#include "domain/DomainUtils.h"

namespace ExternalForces {
	void applyGravity(StaggeredVectorField2D& velocityField, const Domain& domain, float timeStep, float gravity = -9.8f);
}
