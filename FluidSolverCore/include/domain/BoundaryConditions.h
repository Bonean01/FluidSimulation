#pragma once

#include "math/dataStructures/MACGrid.h"
#include "CellData.h"

namespace BoundaryConditions {
	void applyVelocityX(int i, int j, MACGrid2D& velocityField, CellData cellData) {
		switch (cellData.boundaryType) {
			using enum BoundaryType;
			case NoSlip:
				velocityField.setEdgeX(i, j, cellData.prescribedVelocity.x);
				break;
			case FreeSlip:
			case Inlet:
			case Outlet:
				break;
		}
	}

	void applyVelocityY(int i, int j, MACGrid2D& velocityField, CellData cellData) {
		switch (cellData.boundaryType) {
			using enum BoundaryType;
		case NoSlip:
			velocityField.setEdgeY(i, j, cellData.prescribedVelocity.y);
			break;
		case FreeSlip:
		case Inlet:
		case Outlet:
			break;
		}
	}
}
