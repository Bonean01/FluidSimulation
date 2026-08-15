#pragma once

#include "CellType.h"
#include "BoundaryCondition.h"
#include "math/dataStructures/Vector.h"

struct CellData {
	CellType cellType = CellType::Fluid;
	
	CellData(CellType cellType = CellType::Fluid) :
		cellType(cellType) {}
};
