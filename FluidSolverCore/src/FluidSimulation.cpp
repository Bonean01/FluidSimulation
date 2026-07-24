#include "FluidSimulation.h"
#include "math/solvers/PressureSolver.h"

#include <iostream>


void FluidSimulation::step(float dt) {
	advect(m_velocityField, dt);
	advect(m_velocityField, m_smokeField, dt);
	//diffuse(m_velocityField, dt);
	//project(m_velocityField, dt);
	m_velocityField.divergence(m_divergenceField);
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


void FluidSimulation::advect(VectorField2D& velocityField, ScalarField2D& field, float dt) {
	int width = field.width();
	int height = field.height();
	ScalarField2D& auxField = m_auxScalarField;
	if (auxField.width() != width || auxField.height() != height) return;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (isSolid(i, j)) { auxField.setValue(i, j, 0.0f); continue; }
			Vec2f position = { (float)i, (float)j };
			Vec2f currentVel = velocityField.getValue(i, j);
			float newValue = field.sampleBilinear(position - currentVel * dt);
			auxField.setValue(i, j, newValue);
		}
	}
	std::swap(field, auxField);
}


void FluidSimulation::advect(MACGrid2D& field, float dt) {
	int width = field.width();
	int height = field.height();
	MACGrid2D& auxField = m_auxMacGrid;
	if (auxField.width() != width || auxField.height() != height) return;

	for (int i = 0; i < width + 1; i++) {
		for (int j = 0; j < height; j++) {
			if (isSolid(i, j) || isSolid(i - 1, j)) { auxField.setEdgeX(i, j, 0.0f); continue; }
			Vec2f position = { (float)i - 0.5f, (float)j };
			Vec2f currentVel = field.sampleBilinear(position);
			Vec2f newValue = field.sampleBilinear(position - currentVel * dt);
			auxField.setEdgeX(i, j, newValue.x);
		}
	}

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height + 1; j++) {
			if (isSolid(i, j) || isSolid(i, j - 1)) { auxField.setEdgeY(i, j, 0.0f); continue; }
			Vec2f position = { (float)i, (float)j - 0.5f };
			Vec2f currentVel = field.sampleBilinear(position);
			Vec2f newValue = field.sampleBilinear(position - currentVel * dt);
			auxField.setEdgeY(i, j, newValue.y);
		}
	}
	
	std::swap(field, auxField);
}


void FluidSimulation::advect(MACGrid2D& velocityField, ScalarField2D& field, float dt) {
	int width = field.width();
	int height = field.height();
	ScalarField2D& auxField = m_auxScalarField;
	if (auxField.width() != width || auxField.height() != height) return;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (isSolid(i, j)) { auxField.setValue(i, j, 0.0f); continue; }
			Vec2f position = { (float)i, (float)j };
			Vec2f currentVel = velocityField.sampleBilinear(position);
			float newValue = field.sampleBilinear(position - currentVel * dt);
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


void FluidSimulation::project(VectorField2D& field, float dt) {
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


void FluidSimulation::project(MACGrid2D& field, float dt) {
	int width = field.width();
	int height = field.height();

	// Solve the pressure value for every point in the field
	m_pressureSolver.solveJacobi(m_pressureField, dt, m_iterationCount);

	// Subtract the gradient of the pressure field to the velocity field
	for (int i = 0; i < width + 1; i++) {
		for (int j = 0; j < height; j++) {
			if (isSolid(i, j) || isSolid(i - 1, j)) { m_velocityField.setEdgeX(i, j, 0.0f); continue; }
			float pressureLeft = m_pressureField.getValue(i - 1, j);
			float pressureRight = m_pressureField.getValue(i, j);
			float gradientX = pressureRight - pressureLeft;
			float diffX = -gradientX * dt / (m_density * m_cellWidth);
			float currentVel = m_velocityField.getEdgeX(i, j);
			m_velocityField.setEdgeX(i, j, currentVel + diffX);

		}
	}

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height + 1; j++) {
			if (isSolid(i, j) || isSolid(i, j - 1)) { m_velocityField.setEdgeY(i, j, 0.0f); continue; }
			float pressureBottom = m_pressureField.getValue(i, j - 1);
			float pressureTop = m_pressureField.getValue(i, j);
			float gradientY = pressureTop - pressureBottom;
			float diffY = -gradientY * dt / (m_density * m_cellWidth);
			float currentVel = m_velocityField.getEdgeY(i, j);
			m_velocityField.setEdgeY(i, j, currentVel + diffY);
		}
	}
}
