#pragma once

#include "math/dataStructures/VectorField.h"
#include "math/dataStructures/ScalarField.h"
#include "math/dataStructures/Vector.h"

class FluidSimulation {
public:
	FluidSimulation(int gridWidth, int gridHeight, float cellWidth, float density = 1, float kinematicViscosity = 0) :
		m_cellWidth(cellWidth),
		m_density(density),
		m_kinematicViscosity(kinematicViscosity),
		m_velocityField(gridWidth, gridHeight, m_cellWidth),
		m_auxVectorField(gridWidth, gridHeight, m_cellWidth),
		m_pressureField(gridWidth, gridHeight, m_cellWidth),
		m_solidCellMap(gridWidth, gridHeight) {
		
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

	const VectorField2D& getVelocityField() const { return m_velocityField; }
	const ScalarField2D& getPressureField() const { return m_pressureField; }
	const Grid2D<uint8_t>& getSolidCellMap() const { return m_solidCellMap; }

	void setVelocity(int i, int j, Vector2 velocity) { m_velocityField.setValue(i, j, velocity); }
	Vector2 getVelocity(int i, int j) const { return m_velocityField.getValue(i, j); }
	void addVelocity(int i, int j, Vector2 deltaVel) { setVelocity(i, j, getVelocity(i, j) + deltaVel); }
	void setSolidCell(int i, int j, bool isSolid) { m_solidCellMap.setValue(i, j, isSolid); }

	bool isSolid(int i, int j) const { return m_solidCellMap.getValue(i, j); }


private:
	VectorField2D m_velocityField, m_auxVectorField;
	ScalarField2D m_pressureField;
	// 0 => not solid, 1 => solid
	Grid2D<uint8_t> m_solidCellMap;
	float m_density, m_kinematicViscosity, m_cellWidth;
	
	void advect(VectorField2D& field, VectorField2D& auxVectorField, float dt) const;
	void diffuse(VectorField2D& field, VectorField2D& auxVectorField, float dt) const;
	void applyExternalForces(VectorField2D& field);
	void project(VectorField2D& field);

	void pressureAt(int i, int j) {

	}
};
