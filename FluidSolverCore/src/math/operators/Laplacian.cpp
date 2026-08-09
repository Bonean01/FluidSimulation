#include "math/operators/FiniteDifference.h"
#include "math/operators/Staggered.h"


namespace FiniteDifference {
	float Central::laplacian(int i, int j, const ScalarField2D& scalarField) {
		float center = scalarField.getValue(i, j);
		float right = scalarField.getValue(i + 1, j);
		float left = scalarField.getValue(i - 1, j);
		float top = scalarField.getValue(i, j + 1);
		float bottom = scalarField.getValue(i, j - 1);
		float dx = scalarField.cellWidth();

		return (right + left + top + bottom - 4 * center) / (dx * dx);
	}

	Vec2f Central::laplacian(int i, int j, const VectorField2D& vectorField) {
		Vec2f center = vectorField.getValue(i, j);
		Vec2f right = vectorField.getValue(i + 1, j);
		Vec2f left = vectorField.getValue(i - 1, j);
		Vec2f top = vectorField.getValue(i, j + 1);
		Vec2f bottom = vectorField.getValue(i, j - 1);
		float dx = vectorField.cellWidth();
		
		return (right + left + top + bottom - 4 * center) / (dx * dx);
	}
}



namespace Staggered {
	void laplacian(StaggeredVectorField2D& result, const StaggeredVectorField2D& vectorField) {
		int width = vectorField.width();
		int height = vectorField.height();
		float dx = vectorField.cellWidth();
		
		// === horizontal ===
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width + 1; i++) {
				float center = vectorField.getEdgeValue<X>(i, j);
				float right = vectorField.getEdgeValue<X>(i + 1, j);
				float left = vectorField.getEdgeValue<X>(i - 1, j);
				float top = vectorField.getEdgeValue<X>(i, j + 1);
				float bottom = vectorField.getEdgeValue<X>(i, j - 1);

				float x = (right + left + top + bottom - 4 * center) / (dx * dx);
				result.setEdgeValue<X>(i, j, x);
			}
		}
		// === vertical ===
		for (int j = 0; j < height + 1; j++) {
			for (int i = 0; i < width; i++) {
				float center = vectorField.getEdgeValue<Y>(i, j);
				float right = vectorField.getEdgeValue<Y>(i + 1, j);
				float left = vectorField.getEdgeValue<Y>(i - 1, j);
				float top = vectorField.getEdgeValue<Y>(i, j + 1);
				float bottom = vectorField.getEdgeValue<Y>(i, j - 1);

				float y = (right + left + top + bottom - 4 * center) / (dx * dx);
				result.setEdgeValue<Y>(i, j, y);
			}
		}
	}
}
