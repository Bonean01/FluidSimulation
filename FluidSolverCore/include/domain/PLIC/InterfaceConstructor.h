#include "FluidInterface.h"
#include "math/dataStructures/ScalarField.h"

class InterfaceConstructor {
public:
    FluidInterface2D construct(const ScalarField2D& volumeFractionField);

private:
    VectorComponent getMajorAxisAt(int i, int j);
    void computeNormalAt(int i, int j, const VectorComponent& majorAxis);
};
