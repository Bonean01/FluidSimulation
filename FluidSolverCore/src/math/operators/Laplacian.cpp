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
		
		return Vec2f{
			(right.x + left.x + top.x + bottom.x - 4 * center.x),
			(right.y + left.y + top.y + bottom.y - 4 * center.y)
		} / (dx * dx);
	}
}



namespace Staggered {
	void laplacian(MACGrid2D& result, const MACGrid2D& vectorField) {
		int width = vectorField.width();
		int height = vectorField.height();

		ScalarField2D divergence{ width, height, vectorField.cellWidth() };
		Staggered::divergence(divergence, vectorField);
		
		// === horizontal ===
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width + 1; i++) {
				float x = gradientX(i, j, divergence);
				result.setEdgeX(i, j, x);
			}
		}
		// === vertical ===
		for (int j = 0; j < height + 1; j++) {
			for (int i = 0; i < width; i++) {
				float y = gradientY(i, j, divergence);
				result.setEdgeY(i, j, y);
			}
		}
	}
}
