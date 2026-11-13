#include "math/operators/FiniteDifference.h"

namespace FiniteDifference {
    // Computes the surface divergence of the normals: κ = -∇_s * n for a given cell
    float curvature(int i, int j, const FluidInterface2D& fluidInterface) {
        auto intersections = fluidInterface.getInterfaceIntersectionsWithCell(i, j);
        Vec2f intersectionA = intersections.first;
        Vec2f intersectionB = intersections.second;
        Vec2f position = static_cast<Vec2f>(Vec2i{ i, j });

        Vec2f normalA = fluidInterface.sampleNormalBilinear(position + intersectionA);
        Vec2f normalB = fluidInterface.sampleNormalBilinear(position + intersectionB);

        float deltaLength = (intersectionB - intersectionA).magnitude();

        // κ = sign(cross(n_a, n_b)) * magnitude((n_b - n_a) / ΔS)
        float sign = std::signbit(Vec2f::cross(normalA, normalB)) ? -1.0f : 1.0f;
        float curvature = sign * ((normalB - normalA) / deltaLength).magnitude();

        return curvature;
    }

    void curvature(ScalarField2D& result, const FluidInterface2D& fluidInterface) {
        int width = fluidInterface.width();
        int height = fluidInterface.height();

        #pragma omp parallel for
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                Vec2f normal = fluidInterface.getValue(i, j).normal;
                if (normal.x == 0.0f && normal.y == 0.0f) {
                    result.setValue(i, j, 0.0f);
                    continue;
                }
                float curvature = FiniteDifference::curvature(i, j, fluidInterface);
                result.setValue(i, j, curvature);
            }
        }
    }
};
