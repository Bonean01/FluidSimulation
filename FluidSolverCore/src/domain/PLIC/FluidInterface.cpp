#include "domain/PLIC/FluidInterface.h"
#include <array>
#include "math/operators/FiniteDifference.h"



void FluidInterface2D::update(const ScalarField2D& volumeFractionField) {
	int width = volumeFractionField.width();
	int height = volumeFractionField.height();
	float cellWidth = volumeFractionField.cellWidth();

	#pragma omp parallel for
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			VectorComponent majorAxis = computeMajorAxisAt(i, j, volumeFractionField);
			Vec2f normal = computeNormalAt(i, j, majorAxis, volumeFractionField);
			float volumeFraction = volumeFractionField.getValue(i, j);
			float intercept = computeIntercept(majorAxis, normal, volumeFraction);

			InterfaceData data{normal, intercept};
			this->setValue(i, j, data);
		}
	}
}



VectorComponent FluidInterface2D::computeMajorAxisAt(int i, int j, const ScalarField2D& volumeFractionField) {
	using enum VectorComponent;

	Vec2f gradient = FiniteDifference::Central::gradient(i, j, volumeFractionField);
	float m_x = gradient.x;
	float m_y = gradient.y;
	return std::abs(m_x) > std::abs(m_y) ? X : Y;
}



Vec2f FluidInterface2D::computeNormalAt(int i, int j, const VectorComponent& majorAxis, const ScalarField2D& volumeFractionField) {
	using enum VectorComponent;
	
	float currentVolumeFraction = volumeFractionField.getValue(i, j);
	
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
	if (majorAxis == X) { normal = { 1.0f, -diff / cellWidth }; }
	else { normal = { -diff / cellWidth, 1.0f }; }

	return normal;
}



/* Computes the intercept for the specific case of a normal with positive components */
float FluidInterface2D::computeIntercept(float slope, float volumeFraction) {
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



float FluidInterface2D::computeIntercept(const VectorComponent& majorAxis, const Vec2f& normal, float volumeFraction) {
	using enum VectorComponent;

	float slope = majorAxis == X ? normal.get(Y) : normal.get(X);
	if (majorAxis == X) {
		if (slope >= 0) return computeIntercept(slope, volumeFraction) / slope;
		else return 1 + computeIntercept(-slope, volumeFraction) / slope;
	}
	else {
		if (slope >= 0) return computeIntercept(slope, volumeFraction);
		else return computeIntercept(-slope, volumeFraction) + slope;
	}
}



float FluidInterface2D::computeFluxAt(const VectorComponent& C, int i, int j, const StaggeredVectorField2D& velocityField, const ScalarField2D& volumeFractionField, float cellWidth, float timeStep) {
    using enum VectorComponent;

    float speed = velocityField.getEdgeValue(C, i, j);
    float scaledSpeed = timeStep / cellWidth * speed;
    if (speed < 0) {
        if (C == X) {
            // left
        }
        else {
            // bottom
        }
    } 
    else if (speed > 0) {
        if (C == X) {
            // right
        }
        else {
            // top
        }
    }
    else return 0.0f;
}




// Note: Maybe unify InterfaceConstructor into FluidInterface, that way the major axis can be accessed at any time

/* Computes flux for the specific case of a downward velocity 
* PRECONDITION: speed must be scaled by Δt / Δx
*/
float FluidInterface2D::computeFlux(float volumeFraction, float scaledSpeed, float intercept, float slope, bool isFluidBellow) {
    float fluxBellow = intersectsVelocity(scaledSpeed, intercept, slope) 
        ? lowerBound(volumeFraction, scaledSpeed, isFluidBellow)
        : upperBound(volumeFraction, scaledSpeed, intercept, isFluidBellow) - computeExcess(scaledSpeed, intercept, slope);
    
    float fluxAbove = scaledSpeed - fluxBellow;

    return isFluidBellow ? fluxBellow : fluxAbove;
}



bool FluidInterface2D::intersectsVelocity(float scaledSpeed, float intercept, float slope) {
    Vec2f intersectionPoint = { (intercept - scaledSpeed) / slope, scaledSpeed };
    return 0 < intersectionPoint.x && intersectionPoint.x < 1
        && 0 < intersectionPoint.y && intersectionPoint.y < 1;
}



float FluidInterface2D::lowerBound(float volumeFraction, float scaledSpeed, bool isFluidBellow) {
    return std::min(isFluidBellow ? volumeFraction : 1 - volumeFraction, scaledSpeed);
}



float FluidInterface2D::upperBound(float volumeFraction, float scaledSpeed, float intercept, bool isFluidBellow) {
    if (intercept < scaledSpeed)
        return scaledSpeed;
    else
        return isFluidBellow ? volumeFraction : 1 - volumeFraction;
}



float FluidInterface2D::computeExcess(float scaledSpeed, float intercept, float slope) {
    float min = std::min(scaledSpeed, intercept);
    float max = std::max(scaledSpeed, intercept);
    float a = std::max(0.0f, min);
    float b = std::min(max, 1.0f);

    return (b - a) * (2 * intercept - a - b) / (2 * slope);
}
