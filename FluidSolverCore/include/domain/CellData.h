#pragma once

#include "CellType.h"
#include "BoundaryType.h"
#include "math/dataStructures/Vector.h"

struct CellData {
	CellType cellType = CellType::Fluid;

	CellData() {}
	
	CellData(CellType cellType) :
		cellType(cellType) {}
};
