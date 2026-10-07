#include "FluidInterface.h"
#include "math/dataStructures/ScalarField.h"

class InterfaceConstructor {
public:
    FluidInterface2D construct(const ScalarField2D& volumeFractionField);

private:
    VectorComponent getMajorAxisAt(int i, int j, const ScalarField2D& volumeFractionField);
    Vec2f computeNormalAt(int i, int j, const VectorComponent& majorAxis, const ScalarField2D& volumeFractionField);
    float computeIntercept(const Vec2f& normal, float volumeFraction);
};
