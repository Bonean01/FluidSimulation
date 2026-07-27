#include "math/operators/FiniteDifference.h"


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
