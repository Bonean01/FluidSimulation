#pragma once

#include <cstdlib>
#include <cstdint>
#include <cmath>

#include "math/dataStructures/MACGrid.h"
#include "math/dataStructures/Grid.h"

namespace TestUtils {
	void initializeRandomVelocities(MACGrid2D& velocityField, float min, float max, unsigned int seed);
	void initializeConstantVelocities(MACGrid2D& velocityField, float constant);

	void initializeSolidBoundaries(Grid2D<uint8_t>& solidCellMap);
}
