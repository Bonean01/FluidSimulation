#pragma once

#include "math/dataStructures/ScalarField.h"
#include "math/dataStructures/StaggeredVectorField.h"

namespace Staggered {
	float gradientX(int i, int j, const ScalarField2D& scalarField);
	float gradientY(int i, int j, const ScalarField2D& scalarField);
	float divergence(int i, int j, const StaggeredVectorField2D& vectorField);
	void divergence(ScalarField2D& result, const StaggeredVectorField2D& vectorField);
	void laplacian(StaggeredVectorField2D&result, const StaggeredVectorField2D& vectorField);
}
