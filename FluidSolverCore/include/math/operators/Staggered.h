#pragma once

#include "math/dataStructures/ScalarField.h"
#include "math/dataStructures/StaggeredVectorField.h"

namespace Staggered {
	float divergence(int i, int j, const StaggeredVectorField2D& vectorField);
	void divergence(ScalarField2D& result, const StaggeredVectorField2D& vectorField);
	void laplacian(StaggeredVectorField2D&result, const StaggeredVectorField2D& vectorField);

	template<VectorComponent C>
	float gradient(int i, int j, const ScalarField2D& scalarField) {
		float dx = scalarField.cellWidth();
		if constexpr (C == X) {
			float left = scalarField.getValue(i - 1, j);
			float right = scalarField.getValue(i, j);
			return (right - left) / dx;
		}
		else if constexpr (C == Y) {
			float bottom = scalarField.getValue(i, j - 1);
			float top = scalarField.getValue(i, j);
			return (top - bottom) / dx;
		}
		else
			throw std::runtime_error(
            	"StaggeredGrid2D only has components X and Y but accessed: " + C
        	);
	}
}
