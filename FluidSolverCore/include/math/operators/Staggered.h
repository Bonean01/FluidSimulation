#pragma once

#include "math/dataStructures/ScalarField.h"
#include "math/dataStructures/StaggeredVectorField.h"

namespace Staggered {
	float divergence(int i, int j, const StaggeredVectorField2D& vectorField);
	void divergence(ScalarField2D& result, const StaggeredVectorField2D& vectorField);
	void laplacian(StaggeredVectorField2D&result, const StaggeredVectorField2D& vectorField);
	float gradient(const VectorComponent& C, int i, int j, const ScalarField2D& scalarField);
}
