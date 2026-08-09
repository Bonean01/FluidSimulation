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
	static void laplacian(const VectorComponent& C, StaggeredVectorField2D& result, const StaggeredVectorField2D& vectorField) {
		int width = vectorField.getValuesWidth(C);
		int height = vectorField.getValuesHeight(C);
		float dx = vectorField.cellWidth();
		
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				float center = vectorField.getEdgeValue(C, i, j);
				float right = vectorField.getEdgeValue(C, i + 1, j);
				float left = vectorField.getEdgeValue(C, i - 1, j);
				float top = vectorField.getEdgeValue(C, i, j + 1);
				float bottom = vectorField.getEdgeValue(C, i, j - 1);

				float res = (right + left + top + bottom - 4 * center) / (dx * dx);
				result.setEdgeValue(C, i, j, res);
			}
		}
	}

	void laplacian(StaggeredVectorField2D& result, const StaggeredVectorField2D& vectorField) {
		int width = vectorField.width();
		int height = vectorField.height();
		float dx = vectorField.cellWidth();
		
		laplacian(X, result, vectorField);
		laplacian(Y, result, vectorField);
	}
}
