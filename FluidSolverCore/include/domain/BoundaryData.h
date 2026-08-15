#pragma once

#include "BoundaryCondition.h"
#include "math/dataStructures/Vector.h"

struct BoundaryData {
    BoundaryCondition velocityBoundaryCondition = BoundaryCondition::None;
    Vec2f prescribedVelocity = { 0.0f, 0.0f };

    BoundaryData(BoundaryCondition velocityBoundaryCondition = BoundaryCondition::None, Vec2f prescribedVelocity = { 0.0f, 0.0f }) :
        velocityBoundaryCondition(velocityBoundaryCondition),
        prescribedVelocity(prescribedVelocity) {}
};
