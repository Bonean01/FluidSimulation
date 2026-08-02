#pragma once

#include "math/dataStructures/MACGrid.h"
#include "CellData.h"


namespace BoundaryConditions {
	void applyVelocity(VectorComponent component, int i, int j, MACGrid2D& velocityField, const CellData& cellData) {
		using enum VectorComponent;

		switch (cellData.boundaryType) {
			using enum BoundaryType;

			case NoSlip:
					velocityField.setEdge(component, i, j, cellData.prescribedVelocity.get(component));
				break;

			case FreeSlip:
			case Inlet:
			case Outlet:
				break;
		}
	}
}
