#pragma once

#include "math/dataStructures/ScalarField.h"
#include "math/dataStructures/VectorField.h"

namespace FiniteDifference {
	namespace Central {
		Vec2f gradient(int i, int j, const ScalarField2D& scalarField);
		void gradient(VectorField2D& result, const ScalarField2D& scalarField);
		float divergence(int i, int j, const VectorField2D& vectorField);
		void divergence(ScalarField2D& result, const VectorField2D& vectorField);
		float laplacian(int i, int j, const ScalarField2D& scalarField);
		Vec2f laplacian(int i, int j, const VectorField2D& vectorField);
	}

	namespace Forward {
		Vec2f gradient(int i, int j, const ScalarField2D& scalarField);
		void gradient(VectorField2D& result, const ScalarField2D& scalarField);
		float divergence(int i, int j, const VectorField2D& vectorField);
		void divergence(ScalarField2D& result, const VectorField2D& vectorField);
	}

	namespace Backward {
		Vec2f gradient(int i, int j, const ScalarField2D& scalarField);
		void gradient(VectorField2D& result, const ScalarField2D& scalarField);
		float divergence(int i, int j, const VectorField2D& vectorField);
		void divergence(ScalarField2D& result, const VectorField2D& vectorField);
	}
}
