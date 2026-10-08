#pragma once

#include "FluidInterface.h"
#include "math/dataStructures/ScalarField.h"

class InterfaceConstructor {
public:
    InterfaceConstructor() {}

    FluidInterface2D construct(const ScalarField2D& volumeFractionField);

private:
    VectorComponent getMajorAxisAt(int i, int j, const ScalarField2D& volumeFractionField);
    Vec2f computeNormalAt(int i, int j, const VectorComponent& majorAxis, const ScalarField2D& volumeFractionField);
    float computeIntercept(const VectorComponent& majorAxis, const Vec2f& normal, float volumeFraction);
    float computeIntercept(float slope, float volumeFraction);

    float triangularCase(float m, float f) { return std::sqrt(2.0f * m * f); }
    float trapezoidalCase(float m, float f) { return f + m / 2.0f; }
    float pentagonalCase(float m, float f) { return m * (1 - std::sqrt(2 * (1 - f) / m)) + 1; }
    float frustumCase(float m, float f) { return m * f + 0.5f; }
};
