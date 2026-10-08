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
	if (majorAxis == X) { normal = { 1.0f, diff / cellWidth }; }
	else { normal = { diff / cellWidth, 1.0f }; }
	normal.normalize();

	return normal;
}



/* Computes the intercept for the specific case of a normal with positive components */
float InterfaceConstructor::computeIntercept(float slope, float volumeFraction) {
	float triangular = triangularCase(slope, volumeFraction);
	float trapezoidal = trapezoidalCase(slope, volumeFraction);
	float pentagonal = pentagonalCase(slope, volumeFraction);
	float frustum = frustumCase(slope, volumeFraction);

	bool isTriValid = triangular / slope <= 1 && triangular <= 1;
	bool isTrapValid = trapezoidal / slope > 1 && trapezoidal <= 1;
	bool isPentValid = pentagonal / slope > 1 && pentagonal > 1;
	bool isFrustumValid = frustum / slope <= 1 && frustum > 1;

	if (isTriValid) return triangular;
	else if (isTrapValid) return trapezoidal;
	else if (isPentValid) return pentagonal;
	else if (isFrustumValid) return frustum;
	else return std::numeric_limits<float>::infinity();
}



float InterfaceConstructor::computeIntercept(const VectorComponent& majorAxis, const Vec2f& normal, float volumeFraction) {
	using enum VectorComponent;

	float slope = normal.get(majorAxis);
	if (majorAxis == X) {
		if (slope >= 0) return computeIntercept(slope, volumeFraction) / slope;
		else return 1 + computeIntercept(-slope, volumeFraction) / slope;
	}
	else {
		if (slope >= 0) return computeIntercept(slope, volumeFraction);
		else return computeIntercept(-slope, volumeFraction) + slope;
	}
}



FluidInterface2D InterfaceConstructor::construct(const ScalarField2D& volumeFractionField) {
	int width = volumeFractionField.width();
	int height = volumeFractionField.height();
	float cellWidth = volumeFractionField.cellCount();
	FluidInterface2D res{width, height, cellWidth};

	#pragma omp parallel for
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			VectorComponent majorAxis = getMajorAxisAt(i, j, volumeFractionField);
			Vec2f normal = computeNormalAt(i, j, majorAxis, volumeFractionField);
			float volumeFraction = volumeFractionField.getValue(i, j);
			float intercept = computeIntercept(majorAxis, normal, volumeFraction);

			InterfaceData data{normal, intercept};
			res.setValue(i, j, data);
		}
	}

	return res;
}
