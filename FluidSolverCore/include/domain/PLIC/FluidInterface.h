#pragma once

#include "math/dataStructures/Vector.h"
#include "math/dataStructures/Grid.h"
#include "math/dataStructures/StaggeredVectorField.h"
#include "math/dataStructures/ScalarField.h"


struct InterfaceData {
    Vec2f normal;
    float intercept;
};


class FluidInterface2D : public Grid2D<InterfaceData> {
public:
    FluidInterface2D(const ScalarField2D& volumeFractionField) 
        : Grid2D(volumeFractionField.width(),
                 volumeFractionField.height(),
                 volumeFractionField.cellWidth()) { this->update(volumeFractionField); }

    void update(const ScalarField2D& volumeFractionField);
    float computeFluxAt(const VectorComponent& C, int i, int j, const StaggeredVectorField2D& velocityField, const ScalarField2D& volumeFractionField, float cellWidth, float timeStep);


private:
    VectorComponent computeMajorAxisAt(int i, int j, const ScalarField2D& volumeFractionField);
    Vec2f computeNormalAt(int i, int j, const VectorComponent& majorAxis, const ScalarField2D& volumeFractionField);
    float computeIntercept(const VectorComponent& majorAxis, const Vec2f& normal, float volumeFraction);
    float computeIntercept(float slope, float volumeFraction);

    float triangularCase(float m, float f) { return std::sqrt(2.0f * m * f); }
    float trapezoidalCase(float m, float f) { return f + m / 2.0f; }
    float pentagonalCase(float m, float f) { return m * (1 - std::sqrt(2 * (1 - f) / m)) + 1; }
    float frustumCase(float m, float f) { return m * f + 0.5f; }

    Vec2i getCellPos(const VectorComponent& C, int i, int j, float speed);
    bool isFluidBelow(const Vec2f& normal, const ScalarField2D& volumeFractionField);
    
    float computeFlux(float volumeFraction, float scaledSpeed, float intercept, float slope, bool isFluidBelow);
    bool intersectsVelocity(float scaledSpeed, float intercept, float slope);
    float lowerBound(float volumeFraction, float scaledSpeed, bool isFluidBelow);
    float upperBound(float volumeFraction, float scaledSpeed, float intercept, bool isFluidBelow);
    float computeExcess(float scaledSpeed, float intercept, float slope);
};
