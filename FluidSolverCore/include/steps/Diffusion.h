#pragma once

#include "math/dataStructures/MACGrid.h"
#include "math/dataStructures/Grid.h"
#include "domain/CellData.h"

class Diffusion {
public:
	Diffusion(int gridWidth, int gridHeight, float cellWidth) : 
		m_laplacianField(gridWidth, gridHeight, cellWidth),
		m_auxMacGrid(gridWidth, gridHeight, cellWidth) {}

	void execute(MACGrid2D& velocityField, const Grid2D<CellData>& cellData, float kinematicViscosity, float timeStep, unsigned int iterationCount);

private:
	MACGrid2D m_laplacianField, m_auxMacGrid;
};
