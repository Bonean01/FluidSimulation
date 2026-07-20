#include "FluidSimulation.h"
#include "math/solvers/PressureSolver.h"

#include <iostream>


void FluidSimulation::step(float dt) {
	advect(m_velocityField, dt);
	//diffuse(m_velocityField, dt);
	project(m_velocityField);
	m_projectionError = m_pressureSolver.calculateError();
}


void FluidSimulation::advect(VectorField2D& field, float dt) {
	int width = field.width();
	int height = field.height();
	VectorField2D& auxField = m_auxVectorField;
	if (auxField.width() != width || auxField.height() != height) return;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (isSolid(i, j)) { auxField.setValue(i, j, { 0.0f, 0.0f }); continue; }
			Vec2f position = { (float)i, (float)j };
			Vec2f currentVel = field.getValue(i, j);
			Vec2f newValue = field.sampleBilinear(position - currentVel * dt);
			auxField.setValue(i, j, newValue);
		}
	}
	std::swap(field, auxField);
}


void FluidSimulation::diffuse(VectorField2D& field, float dt) {
	int width = field.width();
	int height = field.height();
	VectorField2D& auxField = m_auxVectorField;
	if (auxField.width() != width || auxField.height() != height) return;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			
		}
	}
}


void FluidSimulation::project(VectorField2D& field) {
	int width = field.width();
	int height = field.height();
	ScalarField2D& auxField = m_auxScalarField;
	if (auxField.width() != width || auxField.height() != height) return;

	// Solve the pressure value for every point in the field
	m_pressureSolver.solve(m_pressureField, m_iterationCount);

	// Subtract the gradient of the pressure field to the velocity field
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Vec2f gradient = m_pressureField.gradient(i, j);
			addVelocity(i, j, -gradient);
		}
	}
}
