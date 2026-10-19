#include "domain/PLIC/FluidInterface.h"
#include <array>
#include "math/operators/FiniteDifference.h"



void FluidInterface2D::reconstruct(const ScalarField2D& volumeFractionField) {
	#pragma omp parallel for
	for (int j = 0; j < m_height; j++) {
		for (int i = 0; i < m_width; i++) {
			VectorComponent majorAxis = computeMajorAxisAt(i, j, volumeFractionField);
			Vec2f normal = computeNormalAt(i, j, majorAxis, volumeFractionField);
			float volumeFraction = volumeFractionField.getValue(i, j);
			float intercept = computeIntercept(majorAxis, normal, volumeFraction);

			InterfaceData data{normal, intercept};
			this->setValue(i, j, data);
		}
	}
}



VectorComponent FluidInterface2D::computeMajorAxisAt(int i, int j, const ScalarField2D& volumeFractionField) const {
	using enum VectorComponent;

	Vec2f gradient = FiniteDifference::Central::gradient(i, j, volumeFractionField);
	float m_x = gradient.x;
	float m_y = gradient.y;
	return std::abs(m_x) > std::abs(m_y) ? X : Y;
}



Vec2f FluidInterface2D::computeNormalAt(int i, int j, const VectorComponent& majorAxis, const ScalarField2D& volumeFractionField) const {
	using enum VectorComponent;
	
	float currentVolumeFraction = volumeFractionField.getValue(i, j);
	if (currentVolumeFraction == 0.0f || currentVolumeFraction == 1.0f) return { 0.0f, 0.0f };
	
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
			aproxLengths[static_cast<size_t>(k) + 1] += volumeFraction * cellWidth;
		}
	}

	float diff;
	if (currentVolumeFraction > 0.5f) {
		// Use right + center (forward diff)
		diff = aproxLengths[2] - aproxLengths[1];
	}
	else {
		// Use center + left (backward diff)
		diff = aproxLengths[1] - aproxLengths[0];
	}

	Vec2f normal;
	Vec2f gradient = FiniteDifference::Central::gradient(i, j, volumeFractionField);
	if (majorAxis == X) { normal = { 1.0f * gradient.x < 0 ? 1.0f : -1.0f, -diff / cellWidth }; }
	else { normal = { -diff / cellWidth, 1.0f * gradient.y < 0 ? 1.0f : -1.0f }; }

	return normal;
}



/* Computes the intercept for the specific case of a normal with positive components */
float FluidInterface2D::computeIntercept(float slope, float volumeFraction) const {
	if (volumeFraction == 0 || volumeFraction == 1) return 0.0f;

	slope = std::abs(slope) + std::numeric_limits<float>::epsilon();

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



float FluidInterface2D::computeIntercept(const VectorComponent& majorAxis, const Vec2f& normal, float volumeFraction) const {
	using enum VectorComponent;

	float slope = majorAxis == X ? normal.y : normal.x;
	if (majorAxis == X) {
		if (slope >= 0) return computeIntercept(slope, volumeFraction) / slope;
		else return 1 + computeIntercept(-slope, volumeFraction) / slope;
	}
	else {
		if (slope >= 0) return computeIntercept(slope, volumeFraction);
		else return computeIntercept(-slope, volumeFraction) + slope;
	}
}



Vec2i FluidInterface2D::getCellPos(const VectorComponent& C, int i, int j, float velocity) const {
    using enum VectorComponent;

    if (velocity > 0)
        if (C == X) return { i - 1, j };
        else return { i, j - 1 };
    else return { i, j };
}



float FluidInterface2D::computeFluxAt(const VectorComponent& C, int i, int j, const StaggeredVectorField2D& velocityField, const ScalarField2D& volumeFractionField, float cellWidth, float timeStep) const {
    using enum VectorComponent;

    float velocity = velocityField.getEdgeValue(C, i, j);
    float scaledSpeed = std::abs(timeStep / cellWidth * velocity);

    Vec2i cellPos = getCellPos(C, i, j, velocity);
    InterfaceData data = getValue(cellPos);
    VectorComponent majorAxis = computeMajorAxisAt(cellPos.x, cellPos.y, volumeFractionField);
    
    float volumeFraction = volumeFractionField.getValue(cellPos);
    float intercept = data.intercept;
	float slope = majorAxis == X ? 1 / data.normal.y : data.normal.x;
    bool isFluidBelow = data.normal.y > 0;

	float normalizedFlux = 0.0f;

    // left or bottom
    if (velocity < 0) {
        if (C == X) normalizedFlux = -computeNormalizedFlux(volumeFraction, scaledSpeed, (intercept - 1) / slope, -1 / slope, slope > 0 ? isFluidBelow : !isFluidBelow);
        else normalizedFlux = -computeNormalizedFlux(volumeFraction, scaledSpeed, intercept, slope, isFluidBelow);
    }
    // right and top
    else if (velocity > 0) {
        if (C == X) normalizedFlux = computeNormalizedFlux(volumeFraction, scaledSpeed, 1 - intercept / slope, -1 / slope, slope < 0 ? isFluidBelow : !isFluidBelow);
        else normalizedFlux = computeNormalizedFlux(volumeFraction, scaledSpeed, 1 - intercept, -slope, !isFluidBelow);
    }

	float cellVolume = cellWidth * cellWidth;
	return normalizedFlux * cellVolume / timeStep;
}



/* Computes flux scaled by Δt / ΔΩ for the specific case of a downward velocity
* PRECONDITION: speed must be scaled by Δt / Δx
*/
float FluidInterface2D::computeNormalizedFlux(float volumeFraction, float scaledSpeed, float intercept, float slope, bool isFluidBelow) const {
	if (volumeFraction == 0.0f) return 0.0f;
	if (volumeFraction == 1.0f) return scaledSpeed;

    float fluxBelow = intersectsVelocity(scaledSpeed, intercept, slope) 
        ? upperBound(volumeFraction, scaledSpeed, intercept, isFluidBelow) - computeExcess(scaledSpeed, intercept, slope)
        : lowerBound(volumeFraction, scaledSpeed, isFluidBelow);
    
    float fluxAbove = scaledSpeed - fluxBelow;

    return isFluidBelow ? fluxBelow : fluxAbove;
}



bool FluidInterface2D::intersectsVelocity(float scaledSpeed, float intercept, float slope) const {
    Vec2f intersectionPoint = { (intercept - scaledSpeed) / slope, scaledSpeed };
    return 0 < intersectionPoint.x && intersectionPoint.x < 1
        && 0 < intersectionPoint.y && intersectionPoint.y < 1;
}



float FluidInterface2D::lowerBound(float volumeFraction, float scaledSpeed, bool isFluidBelow) const {
    return std::min(isFluidBelow ? volumeFraction : 1 - volumeFraction, scaledSpeed);
}



float FluidInterface2D::upperBound(float volumeFraction, float scaledSpeed, float intercept, bool isFluidBelow) const {
    if (intercept < scaledSpeed)
        return scaledSpeed;
    else
        return isFluidBelow ? volumeFraction : 1 - volumeFraction;
}



float FluidInterface2D::computeExcess(float scaledSpeed, float intercept, float slope) const {
    float min = std::min(scaledSpeed, intercept);
    float max = std::max(scaledSpeed, intercept);
    float a = std::max(0.0f, min);
    float b = std::min(max, 1.0f);

    return (b - a) * (2 * intercept - a - b) / (2 * slope);
}
