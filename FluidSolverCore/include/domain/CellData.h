#pragma once

#include "CellType.h"
#include "BoundaryType.h"
#include "math/dataStructures/Vector.h"

struct CellData {
	CellType cellType = CellType::Fluid;
	BoundaryType boundaryType = BoundaryType::None;
	bool hasPrescribedVelocity = false;
	Vec2f prescribedVelocity = 0.0f;
	bool hasPrescribedPressure = false;
	float prescribedPressure = 0.0f;

	CellData() {}
	
	CellData(CellType cellType, BoundaryType boundaryType) :
		cellType(cellType),
		boundaryType(boundaryType) { hasPrescribedVelocity = false; }

	CellData(CellType cellType, BoundaryType boundaryType, Vec2f prescribedVelocity) :
		cellType(cellType),
		boundaryType(boundaryType),
		prescribedVelocity(prescribedVelocity) { hasPrescribedVelocity = true; }
};
