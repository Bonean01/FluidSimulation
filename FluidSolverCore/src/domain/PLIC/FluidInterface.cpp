#include "domain/PLIC/FluidInterface.h"


float FluidInterface2D::computeFluxAt(const VectorComponent& C, int i, int j, const StaggeredVectorField2D& velocityField, const ScalarField2D& volumeFractionField, float cellWidth, float timeStep) {
    using enum VectorComponent;

    float speed = velocityField.getEdgeValue(C, i, j);
    float scaledSpeed = timeStep / cellWidth * speed;
    if (speed < 0) {
        if (C == X) {
            float volumeFraction = volumeFractionField.getValue(i, j);
            InterfaceData data = getValue(i, j);
            //float slope = majorAxis == X ? -data.normal.y : -data.normal.x;
            bool isFluidBellow = true; // use the normal to determine this
            //float flux = flux(volumeFraction, scaledSpeed, data.intercept, slope, isFluidBellow);
        }
    }
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
