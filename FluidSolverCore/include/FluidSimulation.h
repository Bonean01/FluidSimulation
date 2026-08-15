#pragma once

#include "math/dataStructures/Grid.h"
#include "math/dataStructures/StaggeredGrid.h"
#include "math/dataStructures/Vector.h"
#include "math/dataStructures/ScalarField.h"
#include "math/dataStructures/VectorField.h"

#include "math/solvers/PressureSolver.h"

#include "steps/Advection.h"
#include "steps/Projection.h"
#include "steps/Diffusion.h"

#include "domain/CellData.h"
#include "domain/BoundaryData.h"
#include "domain/CellConfig.h"


class FluidSimulation {
public:
	FluidSimulation(int gridWidth, int gridHeight, float cellWidth, float density = 1, float kinematicViscosity = 0, unsigned int iterationCount = 60) :
		m_cellWidth(cellWidth),
		m_density(density),
		m_kinematicViscosity(kinematicViscosity),

		m_velocityField(gridWidth, gridHeight, m_cellWidth),
		m_pressureField(gridWidth, gridHeight, m_cellWidth),
		m_divergenceField(gridWidth, gridHeight, m_cellWidth),
		m_smokeField(gridWidth, gridHeight, m_cellWidth),
		m_cellData(gridWidth, gridHeight),
		m_boundaryData(gridWidth, gridHeight),

		m_advection(gridWidth, gridHeight, m_cellWidth),
		m_diffusion(gridWidth, gridHeight, m_cellWidth),
		m_projection(),

		m_pressureSolver(gridWidth, gridHeight, m_cellWidth),
		m_iterationCount(iterationCount) {
	}

	void step(float timeStep);

	const StaggeredVectorField2D getVelocityField() const { return m_velocityField; }
	const ScalarField2D& getPressureField() const { return m_pressureField; }
	const ScalarField2D& getDivergenceField() const { return m_divergenceField; }
	const ScalarField2D& getSmokeField() const { return m_smokeField; }
	const Grid2D<CellData>& getCellData() const { return m_cellData; }

	const float getDensity() const { return m_density; }
	const float getKinematicViscosity() const { return m_kinematicViscosity; }
	const float getCellWidth() const { return m_cellWidth; }

	void setVelocity(int i, int j, Vec2f velocity) { m_velocityField.setCellValue(i, j, velocity); }
	Vec2f getVelocity(int i, int j) const { return m_velocityField.getCellValue(i, j); }
	void addVelocity(int i, int j, Vec2f deltaVel) { setVelocity(i, j, getVelocity(i, j) + deltaVel); }

	void addSmoke(int i, int j, float deltaSmoke) { m_smokeField.setValue(i, j, m_smokeField.getValue(i, j) + deltaSmoke); }

	void setCell(int i, int j, CellData, BoundaryData);
	void setCell(int i, int j, CellConfig config) { setCell(i, j, config.cellData, config.boundaryData); }


private:
	float m_density, m_kinematicViscosity, m_cellWidth;

	StaggeredVectorField2D m_velocityField;
	ScalarField2D m_pressureField, m_divergenceField, m_smokeField;
	Grid2D<CellData> m_cellData;
	StaggeredGrid2D<BoundaryData> m_boundaryData;

	Advection m_advection;
	Projection m_projection;
	Diffusion m_diffusion;

	PressureSolver m_pressureSolver;
	int m_iterationCount;
};
