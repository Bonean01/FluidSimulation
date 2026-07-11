#pragma once

#include "VectorField.h"

class FluidSimulation {
public:
	FluidSimulation(int gridWidth, int gridHeight, float density = 1, float kinematicViscosity = 0) :
	m_density(density),
	m_kinematicViscosity(kinematicViscosity),
	m_velocityField(gridWidth, gridHeight) {}


private:
	VectorField2D m_velocityField;
	float m_density, m_kinematicViscosity;
};
