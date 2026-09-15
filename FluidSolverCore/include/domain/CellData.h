#pragma once

#include "CellType.h"
#include "BoundaryCondition.h"
#include "math/dataStructures/Vector.h"

struct CellData {
	CellType cellType = CellType::Void;
	
	CellData(CellType cellType = CellType::Void) :
		cellType(cellType) {}
};
