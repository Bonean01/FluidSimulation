#pragma once

#include "math/dataStructures/StaggeredVectorField.h"
#include "math/dataStructures/Grid.h"

#include "domain/Domain.h"

namespace ExternalForces {
	void applyGravity(StaggeredVectorField2D& velocityField, const Domain& domain, float timeStep, float gravity = -9.8f);
	void applyContinuumSurfaceForce(StaggeredVectorField2D& velocityField, const Domain& domain, const FluidInterface2D& fluidInterface, const ScalarField2D& volumeFractionField, float density, float timeStep, float surfaceTensionCoefficient = 0.073f);
}
