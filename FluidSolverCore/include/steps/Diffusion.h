#pragma once

#include "math/dataStructures/MACGrid.h"


class Diffusion {
public:
	Diffusion(int gridWidth, int gridHeight, float cellWidth) : 
		m_laplacianField(gridWidth, gridHeight, cellWidth),
		m_auxMacGrid(gridWidth, gridHeight, cellWidth) {}

	void execute(MACGrid2D& velocityField, float kinematicViscosity, float timeStep, int iterationCount);

private:
	MACGrid2D m_laplacianField, m_auxMacGrid;
};
