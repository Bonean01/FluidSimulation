#include "math/operators/FiniteDifference.h"

#include <algorithm>

namespace FiniteDifference {
    float curvature(int i, int j, const FluidInterface2D& fluidInterface) {
        auto intersections = fluidInterface.getInterfaceIntersectionsWithCell(i, j);
        Vec2f intersectionA = intersections.first;
        Vec2f intersectionB = intersections.second;

        Vec2f normalA = fluidInterface.sampleNormalBilinear(intersectionA);
        Vec2f normalB = fluidInterface.sampleNormalBilinear(intersectionB);

        float deltaLength = (intersectionB - intersectionA).magnitude();

        // κ = sign(cross(n_a, n_b)) * magnitude((n_b - n_a) / ΔS)
        float sign = std::signbit(Vec2f::cross(normalA, normalB)) ? -1 : 1;
        float curvature = sign * ((normalB - normalA) / deltaLength).magnitude();

        return curvature;
    }
};
