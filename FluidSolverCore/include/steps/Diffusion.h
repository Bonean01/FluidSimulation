#pragma once

#include "math/dataStructures/StaggeredVectorField.h"
#include "math/dataStructures/Grid.h"
#include "domain/CellData.h"
#include "domain/BoundaryUtils.h"

class Diffusion {
public:
	Diffusion(int gridWidth, int gridHeight, float cellWidth) : 
		m_laplacianField(gridWidth, gridHeight, cellWidth),
		m_auxStaggeredVectorField(gridWidth, gridHeight, cellWidth) {}

	void execute(StaggeredVectorField2D& velocityField, const StaggeredGrid2D<BoundaryData>&, float kinematicViscosity, float timeStep, unsigned int iterationCount);

private:
	StaggeredVectorField2D m_laplacianField, m_auxStaggeredVectorField;

	void diffuseComponent(const VectorComponent& C, StaggeredVectorField2D& velocityField, const StaggeredGrid2D<BoundaryData>& boundaryData, float kinematicViscosity, float timeStep, unsigned int iterationCount, float alpha, float beta);
};
