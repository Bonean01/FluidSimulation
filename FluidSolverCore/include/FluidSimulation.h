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
		m_smokeField(gridWidth, gridHeight, m_cellWidth),
		m_solidCellMap(gridWidth, gridHeight),

		m_auxMacGrid(gridWidth, gridHeight, m_cellWidth),
		m_auxVectorField(gridWidth, gridHeight, m_cellWidth),
		m_auxScalarField(gridWidth, gridHeight, m_cellWidth),

		m_pressureSolver(density, kinematicViscosity, m_velocityField, m_solidCellMap),
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
	const ScalarField2D& getSmokeField() const { return m_smokeField; }
	const Grid2D<uint8_t>& getSolidCellMap() const { return m_solidCellMap; }

	const float getDensity() const { return m_density; }
	const float getKinematicViscosity() const { return m_kinematicViscosity; }
	const float getCellWidth() const { return m_cellWidth; }

	void setVelocity(int i, int j, Vec2f velocity) { m_velocityField.setCellValue(i, j, velocity); }
	Vec2f getVelocity(int i, int j) const { return m_velocityField.getCellValue(i, j); }
	void addVelocity(int i, int j, Vec2f deltaVel) { setVelocity(i, j, getVelocity(i, j) + deltaVel); }

	float getPressure(int i, int j) { return m_pressureField.getValue(i, j); }

	void addSmoke(int i, int j, float deltaSmoke) { m_smokeField.setValue(i, j, m_smokeField.getValue(i, j) + deltaSmoke); }
	
	void setSolidCell(int i, int j, bool isSolid) { m_solidCellMap.setValue(i, j, isSolid); }
	bool isSolid(int i, int j) const { return m_solidCellMap.getValue(i, j); }


private:
	float m_density, m_kinematicViscosity, m_cellWidth;

	MACGrid2D m_velocityField;
	MACGrid2D m_auxMacGrid;
	VectorField2D m_auxVectorField;
	ScalarField2D m_pressureField, m_divergenceField, m_smokeField, m_auxScalarField;
	// 0 => not solid, 1 => solid
	Grid2D<uint8_t> m_solidCellMap;

	PressureSolver m_pressureSolver;
	int m_iterationCount;
	
	void advect(MACGrid2D& velocityField, float dt);
	void advect(const MACGrid2D& velocityField, ScalarField2D& field, float dt);
	void diffuse(MACGrid2D& velocityField, float dt);
	void applyExternalForces(VectorField2D& field);
	void project(MACGrid2D& velocityField, float dt);
};
