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
    FluidInterface2D(int width, int height, float cellWidth) 
        : Grid2D(width, height, cellWidth) {}

    float computeFluxAt(const VectorComponent& C, int i, int j, const StaggeredVectorField2D& velocityField, const ScalarField2D& volumeFractionField, float cellWidth, float timeStep);

private:
    float computeFlux(float volumeFraction, float scaledSpeed, float intercept, float slope, bool isFluidBellow);
    bool intersectsVelocity(float scaledSpeed, float intercept, float slope);
    float lowerBound(float volumeFraction, float scaledSpeed, bool isFluidBellow);
    float upperBound(float volumeFraction, float scaledSpeed, float intercept, bool isFluidBellow);
    float computeExcess(float scaledSpeed, float intercept, float slope);
};
