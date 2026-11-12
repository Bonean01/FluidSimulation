#include "math/operators/Staggered.h"


namespace Staggered {
    float curvature(int i, int j, const FluidInterface2D& fluidInterface) {
        // bilinearly sample the vectors (n_a, n_b) at the points where the interface meets the cell edges
        // calculate the lenth of the interface within the cell (ΔS)
        // compute: κ = -∇ * n
        // discretized by: κ = sign(cross(n_a, n_b)) * normalize((n_b - n_a) / ΔS)
    }
};
