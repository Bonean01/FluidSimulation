#pragma once

#include "math/dataStructures/MACGrid.h"
#include "math/dataStructures/ScalarField.h"
#include "CellData.h"
#include "math/dataStructures/Grid.h"


namespace BoundaryUtils {
	void applyVelocityBoundaryConditions(MACGrid2D& velocityField, const Grid2D<CellData>& cellData);
	const bool hasBoundaryPrescribedVelocity(VectorComponent component, const Grid2D<CellData>& cellData, int i, int j);
}
