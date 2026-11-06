#pragma once

#include "CellType.h"
#include "BoundaryCondition.h"
#include "math/dataStructures/Vector.h"

struct CellData {
	CellType cellType;
	bool isInlet;
	
	CellData(CellType cellType = CellType::Empty, bool isInlet = false) :
		cellType(cellType),
		isInlet(isInlet) {}
};
