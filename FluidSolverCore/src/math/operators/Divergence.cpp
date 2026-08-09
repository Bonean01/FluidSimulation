#include "math/operators/FiniteDifference.h"
#include "math/operators/Staggered.h"


namespace FiniteDifference {
	float Central::divergence(int i, int j, const VectorField2D& vectorField) {
		Vec2f right = vectorField.getValue(i + 1, j);
		Vec2f left = vectorField.getValue(i - 1, j);
		Vec2f top = vectorField.getValue(i, j + 1);
		Vec2f bottom = vectorField.getValue(i, j - 1);
		float dx = vectorField.cellWidth();
		return (right.x - left.x + top.y - bottom.y) / (2 * dx);
	}

	void Central::divergence(ScalarField2D& result, const VectorField2D& vectorField) {
		int width = vectorField.width();
		int height = vectorField.height();
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				float divergence = Central::divergence(i, j, vectorField);
				result.setValue(i, j, divergence);
			}
		}
	}
	

	float Forward::divergence(int i, int j, const VectorField2D& vectorField) {
		Vec2f center = vectorField.getValue(i, j);
		Vec2f right = vectorField.getValue(i + 1, j);
		Vec2f top = vectorField.getValue(i, j + 1);
		float dx = vectorField.cellWidth();
		return (right.x - center.x + top.y - center.y) / dx;
	}

	void Forward::divergence(ScalarField2D& result, const VectorField2D& vectorField) {
		int width = vectorField.width();
		int height = vectorField.height();
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				float divergence = Forward::divergence(i, j, vectorField);
				result.setValue(i, j, divergence);
			}
		}
	}


	float Backward::divergence(int i, int j, const VectorField2D& vectorField) {
		Vec2f center = vectorField.getValue(i, j);
		Vec2f left = vectorField.getValue(i - 1, j);
		Vec2f bottom = vectorField.getValue(i, j - 1);
		float dx = vectorField.cellWidth();
		return (center.x - left.x + center.y - bottom.y) / dx;
	}

	void Backward::divergence(ScalarField2D& result, const VectorField2D& vectorField) {
		int width = vectorField.width();
		int height = vectorField.height();
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				float divergence = Backward::divergence(i, j, vectorField);
				result.setValue(i, j, divergence);
			}
		}
	}
}



namespace Staggered {
	float divergence(int i, int j, const MACGrid2D& macGrid) {
		float right = macGrid.getEdgeX(i + 1, j);
		float left = macGrid.getEdgeX(i, j);
		float top = macGrid.getEdgeY(i, j + 1);
		float bottom = macGrid.getEdgeY(i, j);
		float dx = macGrid.cellWidth();
		return (right - left + top - bottom) / dx;
	}
	
	void divergence(ScalarField2D& result, const MACGrid2D& macGrid) {
		int width = macGrid.width();
		int height = macGrid.height();

		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				float divergence = Staggered::divergence(i, j, macGrid);
				result.setValue(i, j, divergence);
			}
		}
	}
	

	float divergence(int i, int j, const StaggeredVectorField2D& vectorField) {
		using enum VectorComponent;
		float right = vectorField.getEdgeValue<X>(i + 1, j);
		float left = vectorField.getEdgeValue<X>(i, j);
		float top = vectorField.getEdgeValue<Y>(i, j + 1);
		float bottom = vectorField.getEdgeValue<Y>(i, j);
		float dx = vectorField.cellWidth();
		return (right - left + top - bottom) / dx;
	}

	void divergence(ScalarField2D& result, const StaggeredVectorField2D& vectorField) {
		int width = vectorField.width();
		int height = vectorField.height();

		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				float divergence = Staggered::divergence(i, j, vectorField);
				result.setValue(i, j, divergence);
			}
		}
	}
}
