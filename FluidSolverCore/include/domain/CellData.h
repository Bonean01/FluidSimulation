#pragma once

#include "CellType.h"
#include "BoundaryType.h"
#include "math/dataStructures/Vector.h"

struct CellData {
	CellType cellType = CellType::Fluid;
	bool hasPrescribedPressure = false;
	float prescribedPressure = 0.0f;

	CellData() {}
	
	CellData(CellType cellType) :
		cellType(cellType) { hasPrescribedPressure = false; }

	CellData(CellType cellType, float prescribedPressure) :
		cellType(cellType),
		prescribedPressure(prescribedPressure) { hasPrescribedPressure = true; }
};
