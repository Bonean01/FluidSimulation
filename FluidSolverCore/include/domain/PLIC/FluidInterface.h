#pragma once

#include "math/dataStructures/Vector.h"
#include "math/dataStructures/Grid.h"


struct InterfaceData {
    Vec2f normal;
    float intercept;
};


class FluidInterface2D : public Grid2D<InterfaceData> {
public:
    FluidInterface2D(int width, int height, float cellWidth) 
        : Grid2D(width, height, cellWidth) {}

    float computeFluxAt(int i, int j);
};
