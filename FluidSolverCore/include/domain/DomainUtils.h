#pragma once

#include "math/dataStructures/StaggeredVectorField.h"
#include "math/dataStructures/ScalarField.h"
#include "math/dataStructures/Grid.h"

#include "BoundaryData.h"
#include "BoundaryCondition.h"
#include "CellData.h"
#include "MarkerParticle.h"
#include "FluidSurface.h"


namespace DomainUtils {
	void applyVelocityBoundaryConditions(StaggeredVectorField2D& velocityField, const StaggeredGrid2D<BoundaryData>& boundaryData);
	void applyVelocityBCsToComponent(const VectorComponent& C, StaggeredVectorField2D& velocityField, const StaggeredGrid2D<BoundaryData>& boundaryData);
	void applyVelocityBCsToEdge(const VectorComponent& C, int i, int j, StaggeredVectorField2D& velocityField, const BoundaryData& boundaryData);
	bool hasBoundaryPrescribedVelocity(const BoundaryData& boundaryData);
	
	void extrapolateVelocity(StaggeredVectorField2D& velocityField, const FluidSurface& surfaceSDF, const Grid2D<CellData>& cellData);
	void updateCellData(Grid2D<CellData>& cellData, const std::vector<MarkerParticle>& markerParticles);
}
