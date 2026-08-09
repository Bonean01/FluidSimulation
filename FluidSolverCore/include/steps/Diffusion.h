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

	void execute(StaggeredVectorField2D& velocityField, const Grid2D<CellData>& cellData, float kinematicViscosity, float timeStep, unsigned int iterationCount);

private:
	StaggeredVectorField2D m_laplacianField, m_auxStaggeredVectorField;

	template<VectorComponent C>
	void diffuseComponent(StaggeredVectorField2D& velocityField, const Grid2D<CellData>& cellData, float kinematicViscosity, float timeStep, unsigned int iterationCount, float alpha, float beta) {
		int width = velocityField.getValuesWidth<C>();
		int height = velocityField.getValuesHeight<C>();

		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				if (BoundaryUtils::hasBoundaryPrescribedVelocity(C, cellData, i, j)) continue;

				float currentVel = velocityField.getEdgeValue<C>(i, j);
				float laplacian = m_laplacianField.getEdgeValue<C>(i, j);
				float newVel = (currentVel + alpha * laplacian) / beta;
				m_auxStaggeredVectorField.setEdgeValue<C>(i, j, newVel);
			}
		}
	}
};
