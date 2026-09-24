#pragma once

#include "math/dataStructures/StaggeredVectorField.h"
#include "math/dataStructures/ScalarField.h"
#include "math/dataStructures/Grid.h"

#include "BoundaryData.h"
#include "BoundaryCondition.h"
#include "CellData.h"
#include "MarkerParticle.h"
#include "FluidSurface.h"
#include "CellConfig.h"


namespace DomainUtils {
	void applyVelocityBoundaryConditions(StaggeredVectorField2D& velocityField, const StaggeredGrid2D<BoundaryData>& boundaryData);
	void applyVelocityBCsToComponent(const VectorComponent& C, StaggeredVectorField2D& velocityField, const StaggeredGrid2D<BoundaryData>& boundaryData);
	void applyVelocityBCsToEdge(const VectorComponent& C, int i, int j, StaggeredVectorField2D& velocityField, const BoundaryData& boundaryData);
	bool hasBoundaryPrescribedVelocity(const BoundaryData& boundaryData);
	bool isFluidEdge(const VectorComponent& C, int i, int j, const Grid2D<CellData>& cellData);
	
	void extrapolateVelocity(StaggeredVectorField2D& velocityField, const FluidSurface& surfaceSDF, const Grid2D<CellData>& cellData);
	void updateCellData(Grid2D<CellData>& cellData, const std::vector<MarkerParticle>& markerParticles);
	void floodDomain(const Grid2D<CellData>&);
}


class Domain {
public:
	Domain(int gridWidth, int gridHeight) :
		m_cellData(gridWidth, gridHeight, 1),
		m_boundaryData(gridWidth, gridHeight, 1),
		m_fluidSurface(gridWidth, gridHeight),
		m_markerParticles(),
		m_extrapolatedVelField(gridWidth, gridHeight, 1) {}

	const Grid2D<CellData>& getCellData() const { return m_cellData; }
	const StaggeredGrid2D<BoundaryData>& getBoundaryData() const  { return m_boundaryData; }
	const FluidSurface& getFluidSurfaceData() const { return m_fluidSurface; }
	std::vector<MarkerParticle>& getMarkerParticles() { return m_markerParticles; }

	void applyVelocityBoundaryConditions(StaggeredVectorField2D& velocityField) const;
	void extrapolateVelocity(StaggeredVectorField2D& velocityField, unsigned int depth);
	bool hasEdgePrescribedVelocity(const VectorComponent& C, int i, int j) const;
	bool isFluidEdge(const VectorComponent& C, int i, int j) const;

	const CellData& getCell(int i, int j) const { return m_cellData.getValue(i, j); }
	const BoundaryData& getBoundary(const VectorComponent& C, int i, int j) const { return m_boundaryData.getEdgeValue(C, i, j); }
	void setCell(int i, int j, const CellConfig& config) { setCell(i, j, config.cellData, config.boundaryData); }
	void setCell(int i, int j, const CellData&, const BoundaryData&);

	void populateMarkerParticles();
	void updateCellData();
	void flood();
	void drain();


private:
	Grid2D<CellData> m_cellData;
	StaggeredGrid2D<BoundaryData> m_boundaryData;
	FluidSurface m_fluidSurface;
	std::vector<MarkerParticle> m_markerParticles;
	VectorField2D m_extrapolatedVelField;

	void applyVelocityBCsToComponent(const VectorComponent& C, StaggeredVectorField2D& velocityField, const StaggeredGrid2D<BoundaryData>& boundaryData) const;
	void applyVelocityBCsToEdge(const VectorComponent& C, int i, int j, StaggeredVectorField2D& velocityField, const BoundaryData& boundaryData) const;
};
