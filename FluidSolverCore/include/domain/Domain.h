#pragma once

#include <vector>

#include "math/dataStructures/ScalarField.h"
#include "math/dataStructures/VectorField.h"
#include "math/dataStructures/StaggeredVectorField.h"
#include "math/dataStructures/Grid.h"
#include "math/dataStructures/StaggeredGrid.h"

#include "BoundaryData.h"
#include "BoundaryCondition.h"
#include "CellData.h"
#include "MarkerParticle.h"
#include "FluidSurface.h"
#include "CellConfig.h"


class Domain {
public:
	Domain(int gridWidth, int gridHeight) :
		m_cellData(gridWidth, gridHeight, 1),
		m_boundaryData(gridWidth, gridHeight, 1),
		m_extrapolatedVelField(gridWidth, gridHeight, 1) {}

	const Grid2D<CellData>& getCellData() const { return m_cellData; }
	const StaggeredGrid2D<BoundaryData>& getBoundaryData() const  { return m_boundaryData; }

	void applyVelocityBoundaryConditions(StaggeredVectorField2D& velocityField) const;
	void extrapolateVelocity(StaggeredVectorField2D& velocityField, const FluidSurface& fluidSurface);
	bool hasEdgePrescribedVelocity(const VectorComponent& C, int i, int j) const;
	bool isFluidEdge(const VectorComponent& C, int i, int j) const;
	bool isPointInsideSolidCell(const Vec2f&) const;

	const CellData& getCell(int i, int j) const { return m_cellData.getValue(i, j); }
	const BoundaryData& getBoundary(const VectorComponent& C, int i, int j) const { return m_boundaryData.getEdgeValue(C, i, j); }
	void setCell(int i, int j, const CellConfig& config) { setCell(i, j, config.cellData, config.boundaryData); }
	void setCell(int i, int j, const CellData&, const BoundaryData&);
	
	void createMarkerParticles(std::vector<MarkerParticle>& markerParticles);
	void updateCellData(const std::vector<MarkerParticle>& markerParticles);
	void flood();
	void drain();


private:
	Grid2D<CellData> m_cellData;
	StaggeredGrid2D<BoundaryData> m_boundaryData;
	VectorField2D m_extrapolatedVelField;

	void applyVelocityBCsToComponent(const VectorComponent& C, StaggeredVectorField2D& velocityField, const StaggeredGrid2D<BoundaryData>& boundaryData) const;
	void applyVelocityBCsToEdge(const VectorComponent& C, int i, int j, StaggeredVectorField2D& velocityField, const BoundaryData& boundaryData) const;
};
