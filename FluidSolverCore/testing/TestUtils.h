#pragma once

#include <cstdlib>
#include <cstdint>
#include <cmath>

#include "math/dataStructures/StaggeredVectorField.h"
#include "math/dataStructures/Grid.h"

#include "domain/CellData.h"

namespace TestUtils {
	void initializeRandomVelocities(StaggeredVectorField2D& velocityField, float min, float max, unsigned int seed);
	void initializeConstantVelocities(StaggeredVectorField2D& velocityField, float constant);

	void initializeSolidBoundaries(Grid2D<uint8_t>& solidCellMap);
	void initializeSolidBoundaries(Grid2D<CellData>& cellData);
}
