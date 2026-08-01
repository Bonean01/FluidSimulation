#pragma once

#include "CellType.h"
#include "BoundaryType.h"
#include "math/dataStructures/Vector.h"

struct CellData {
	CellType cellType = CellType::Fluid;
	BoundaryType boundaryType = BoundaryType::None;
	Vec2f prescribedVelocity = 0.0f;

	CellData() {}

	CellData(CellType cellType, BoundaryType boundaryType, Vec2f prescribedVelocity) :
		cellType(cellType),
		boundaryType(boundaryType),
		prescribedVelocity(prescribedVelocity) {}
};
