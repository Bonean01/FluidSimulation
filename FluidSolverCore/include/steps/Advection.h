#pragma once

#include "math/dataStructures/StaggeredVectorField.h"
#include "math/dataStructures/Grid.h"
#include "math/dataStructures/ScalarField.h"

#include "domain/CellData.h"
#include "domain/BoundaryUtils.h"

class Advection {
public:
	Advection(int gridWidth, int gridHeight, float cellWidth) :
		m_auxStaggeredVectorField(gridWidth, gridHeight, cellWidth),
		m_auxScalarField(gridWidth, gridHeight, cellWidth) {}

	// self-advection
	void execute(StaggeredVectorField2D& velocityField, const Grid2D<CellData>& cellData, float timeStep);
	void execute(ScalarField2D& field, const StaggeredVectorField2D& velocityField, const Grid2D<CellData>& cellData, float timeStep);


private:
	StaggeredVectorField2D m_auxStaggeredVectorField;
	ScalarField2D m_auxScalarField;

	template<VectorComponent C>
	void advectComponent(StaggeredVectorField2D& velocityField, const Grid2D<CellData>& cellData, float timeStep) {
		int width = velocityField.getValuesWidth<C>();
		int height = velocityField.getValuesHeight<C>();
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				if (BoundaryUtils::hasBoundaryPrescribedVelocity(C, cellData, i, j)) continue;

				Vec2f position = velocityField.getEdgePosition<C>(i, j);
				Vec2f currentVel = velocityField.sampleBilinear(position);
				Vec2f newValue = velocityField.sampleBilinear(position - currentVel * timeStep);
				m_auxStaggeredVectorField.setEdgeValue<C>(i, j, newValue.get(C));
			}
		}
	}
};
