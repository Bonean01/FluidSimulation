#include "domain/PLIC/InterfaceConstructor.h"

#include <array>

#include "math/operators/FiniteDifference.h"


VectorComponent InterfaceConstructor::getMajorAxisAt(int i, int j, const ScalarField2D& volumeFractionField) {
	using enum VectorComponent;

	Vec2f gradient = FiniteDifference::Central::gradient(i, j, volumeFractionField);
	float m_x = gradient.x;
	float m_y = gradient.y;
	return m_x > m_y ? X : Y;
}


Vec2f InterfaceConstructor::computeNormalAt(int i, int j, const VectorComponent& majorAxis, const ScalarField2D& volumeFractionField) {
	using enum VectorComponent;
	
	float cellWidth = volumeFractionField.cellWidth();
	auto aproxLengths = std::array<float, 3>{};

	for (int k = -1; k <= 1; k++) {
		for (int l = -1; l <= 1; l++) {
			int posX = i;
			int posY = j;

			if (majorAxis == X) { posX += l; }
			else { posX += k; }

			if (majorAxis == Y) { posY += l; }
			else { posY += k; }

			float volumeFraction = volumeFractionField.getValue(posX, posY);
			aproxLengths[k + 1] += volumeFraction * cellWidth;
		}
	}

	float diff;
	if (aproxLengths[0] < 0.5f * cellWidth || aproxLengths[0] > 2.5f * cellWidth) {
		// Use center + right (forward diff)
		diff = aproxLengths[2] - aproxLengths[1];
	}
	else {
		// Use left + center (backward diff)
		diff = aproxLengths[1] - aproxLengths[0];
	}

	Vec2f normal;
	if (majorAxis == X) { normal = { 1, diff / cellWidth }; }
	else { normal = { diff / cellWidth, 1 }; }
	normal.normalize();

	return normal;
}
