#include "math/operators/FiniteDifference.h"
#include "math/operators/Staggered.h"


namespace FiniteDifference {
	Vec2f Central::gradient(int i, int j, const ScalarField2D& scalarField) {
		float right = scalarField.getValue(i + 1, j);
		float left = scalarField.getValue(i - 1, j);
		float top = scalarField.getValue(i, j + 1);
		float bottom = scalarField.getValue(i, j - 1);
		float dx = scalarField.cellWidth();
		float x = (right - left) / (2 * dx);
		float y = (top - bottom) / (2 * dx);
		return { x, y };
	}

	void Central::gradient(VectorField2D& result, const ScalarField2D& scalarField) {
		int width = scalarField.width();
		int height = scalarField.height();
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				Vec2f gradient = Central::gradient(i, j, scalarField);
				result.setValue(i, j, gradient);
			}
		}
	}


	Vec2f Forward::gradient(int i, int j, const ScalarField2D& scalarField) {
		float center = scalarField.getValue(i, j);
		float right = scalarField.getValue(i + 1, j);
		float top = scalarField.getValue(i, j + 1);
		float dx = scalarField.cellWidth();
		float x = (right - center) / dx;
		float y = (top - center) / dx;
		return { x, y };
	}

	void Forward::gradient(VectorField2D& result, const ScalarField2D& scalarField) {
		int width = scalarField.width();
		int height = scalarField.height();
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				Vec2f gradient = Forward::gradient(i, j, scalarField);
				result.setValue(i, j, gradient);
			}
		}
	}


	Vec2f Backward::gradient(int i, int j, const ScalarField2D& scalarField) {
		float center = scalarField.getValue(i, j);
		float left = scalarField.getValue(i - 1, j);
		float bottom = scalarField.getValue(i, j - 1);
		float dx = scalarField.cellWidth();
		float x = (center - left) / dx;
		float y = (center - bottom) / dx;
		return { x, y };
	}

	void Backward::gradient(VectorField2D& result, const ScalarField2D& scalarField) {
		int width = scalarField.width();
		int height = scalarField.height();
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				Vec2f gradient = Backward::gradient(i, j, scalarField);
				result.setValue(i, j, gradient);
			}
		}
	}
}



namespace Staggered {
	float gradient(const VectorComponent& C, int i, int j, const ScalarField2D& scalarField) {
		float dx = scalarField.cellWidth();
		if (C == X) {
			float left = scalarField.getValue(i - 1, j);
			float right = scalarField.getValue(i, j);
			return (right - left) / dx;
		}
		else if (C == Y) {
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
