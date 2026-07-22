#pragma once

#include "math/dataStructures/Vector.h"
#include "math/dataStructures/ScalarField.h"
#include "math/dataStructures/VectorField.h"
#include "math/dataStructures/MACGrid.h"
#include "math/solvers/PressureSolver.h"

class FluidSimulation {
public:
	FluidSimulation(int gridWidth, int gridHeight, float cellWidth, float density = 1, float kinematicViscosity = 0, int iterationCount = 60) :
		m_cellWidth(cellWidth),
		m_density(density),
		m_kinematicViscosity(kinematicViscosity),

		m_velocityField(gridWidth, gridHeight, m_cellWidth),
		m_pressureField(gridWidth, gridHeight, m_cellWidth),
		m_divergenceField(gridWidth, gridHeight, m_cellWidth),
		m_solidCellMap(gridWidth, gridHeight),

		m_auxMacGrid(gridWidth, gridHeight, m_cellWidth),
		m_auxVectorField(gridWidth, gridHeight, m_cellWidth),
		m_auxScalarField(gridWidth, gridHeight, m_cellWidth),

		m_pressureSolver(m_velocityField, m_solidCellMap),
		m_iterationCount(iterationCount) {
		
		// Initialize all the outer cells to be solid (boundary condition: u = 0)
		for (int i = 0; i < gridWidth; i++) {
			m_solidCellMap.setValue(i, 0, 1);
			m_solidCellMap.setValue(i, gridHeight - 1, 1);
		}
		for (int j = 0; j < gridHeight; j++) {
			m_solidCellMap.setValue(0, j, 1);
			m_solidCellMap.setValue(gridWidth - 1, j, 1);
		}
	}

	void step(float dt);

	const MACGrid2D& getVelocityField() const { return m_velocityField; }
	const ScalarField2D& getPressureField() const { return m_pressureField; }
	const ScalarField2D& getDivergenceField() const { return m_divergenceField; }
	const Grid2D<uint8_t>& getSolidCellMap() const { return m_solidCellMap; }

	void setVelocity(int i, int j, Vec2f velocity) { m_velocityField.setValue(i, j, velocity); }
	Vec2f getVelocity(int i, int j) const { return m_velocityField.getValue(i, j); }
	void addVelocity(int i, int j, Vec2f deltaVel) { setVelocity(i, j, getVelocity(i, j) + deltaVel); }

	float getPressure(int i, int j) { return m_pressureField.getValue(i, j); }
	
	void setSolidCell(int i, int j, bool isSolid) { m_solidCellMap.setValue(i, j, isSolid); }
	bool isSolid(int i, int j) const { return m_solidCellMap.getValue(i, j); }

	float getProjectionError() { return m_projectionError; };


private:
	MACGrid2D m_velocityField, m_auxMacGrid;
	VectorField2D m_auxVectorField;
	ScalarField2D m_pressureField, m_divergenceField, m_auxScalarField;
	// 0 => not solid, 1 => solid
	Grid2D<uint8_t> m_solidCellMap;

	float m_density, m_kinematicViscosity, m_cellWidth;

	PressureSolver m_pressureSolver;
	int m_iterationCount;
	float m_projectionError = 0;
	
	void advect(VectorField2D& field, float dt);
	void advect(MACGrid2D& field, float dt);
	void diffuse(VectorField2D& field, float dt);
	void applyExternalForces(VectorField2D& field);
	void project(VectorField2D& field);
	void project(MACGrid2D& field, float dt);
};
