#pragma once

#include "math/dataStructures/MACGrid.h"
#include "CellData.h"
#include "math/dataStructures/Grid.h"


namespace BoundaryConditions {
	void applyVelocity(VectorComponent component, int i, int j, MACGrid2D& velocityField, const CellData& cellData);
	void applyVelocity(MACGrid2D& velocityField, Grid2D<CellData> cellData);
	const bool isBoundaryNone(VectorComponent component, const Grid2D<CellData>& cellData, int i, int j);
}
