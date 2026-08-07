#include "domain/BoundaryConditions.h"

// For the time being, boundaries are determined by the cell they belong to, if a cell has BoundaryType::NoSlip, then all its faces will have that 
// boundary condition, if a cell right next to it has the BoundaryType::None, the interface between them will still have NoSlip. As of right now
// it is assumed that two cells with BoundaryTypes different than None cannot exist next to one another.


namespace BoundaryConditions {
	void applyVelocity(VectorComponent component, int i, int j, MACGrid2D& velocityField, const CellData& cellData) {
		using enum VectorComponent;

		switch (cellData.boundaryType) {
			using enum BoundaryType;

			case Inlet:
			case NoSlip:
				velocityField.setEdge(component, i, j, cellData.prescribedVelocity.get(component));
				break;

			case Outlet:
				if (component == VectorComponent::X) {
					float left = velocityField.getEdgeX(i - 1, j);
					velocityField.setEdgeX(i, j, left);
				}		
				break;
		}
	}


	void applyVelocity(MACGrid2D& velocityField, const Grid2D<CellData>& cellData) {
		int width = velocityField.width();
		int height = velocityField.height();
		// -- horizontal --
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width + 1; i++) {
				CellData rightCellData = cellData.getValue(i, j);
				CellData leftCellData = cellData.getValue(i - 1, j);
				
				if (isBoundaryNone(VectorComponent::X, cellData, i, j)) continue;

				CellData cellData = rightCellData.boundaryType != BoundaryType::None ? rightCellData : leftCellData;
				BoundaryConditions::applyVelocity(VectorComponent::X, i, j, velocityField, cellData);
			}
		}
		// -- vertical --
		for (int j = 0; j < height + 1; j++) {
			for (int i = 0; i < width; i++) {
				CellData rightCellData = cellData.getValue(i, j);
				CellData leftCellData = cellData.getValue(i, j - 1);

				if (isBoundaryNone(VectorComponent::Y, cellData, i, j)) continue;

				CellData cellData = rightCellData.boundaryType != BoundaryType::None ? rightCellData : leftCellData;
				BoundaryConditions::applyVelocity(VectorComponent::Y, i, j, velocityField, cellData);
			}
		}
	}


	void applyPressure(ScalarField2D& pressureField, const Grid2D<CellData>& cellData) {
		int width = pressureField.width();
		int height = pressureField.height();
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				CellData currentData = cellData.getValue(i, j);

				switch (currentData.boundaryType) {
					using enum BoundaryType;

					case Outlet:
						pressureField.setValue(i, j, 0.0f);
						break;
				}
			}
		}
	}


	const bool isBoundaryNone(VectorComponent component, const Grid2D<CellData>& cellData, int i, int j) {
		if (component == VectorComponent::X)
			return cellData.getValue(i, j).cellType == BoundaryType::None
			&& cellData.getValue(i - 1, j).boundaryType == BoundaryType::None;

		if (component == VectorComponent::Y)
			return cellData.getValue(i, j).boundaryType == BoundaryType::None
			&& cellData.getValue(i, j - 1).boundaryType == BoundaryType::None;
		else
			return true;
	}
}
