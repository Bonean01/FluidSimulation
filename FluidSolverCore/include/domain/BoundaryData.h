#pragma once

#include "BoundaryType.h"
#include "math/dataStructures/Vector.h"

struct BoundaryData {
    BoundaryType boundaryType = BoundaryType::None;
    bool hasPrescribedVelocity = false;
    Vec2f prescribedVelocity = 0.0f;

    BoundaryData() {}

    BoundaryData(BoundaryType boundaryType) :
        boundaryType(boundaryType) { hasPrescribedVelocity = false; }

    BoundaryData(BoundaryType boundaryType, Vec2f prescribedVelocity) :
        boundaryType(boundaryType),
        prescribedVelocity(prescribedVelocity) { hasPrescribedVelocity = true; }
};
