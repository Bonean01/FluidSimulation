#pragma once

#include "math/dataStructures/MACGrid.h"


class Diffusion {
public:
	void execute(MACGrid2D& velocityField, float timeStep);
};
