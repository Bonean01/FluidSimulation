#include "FluidSimulation.h"

void FluidSimulation::step(float dt) {
	// All of the steps within the step function require an auxiliary vectorfield to which to write
	advect(m_velocityField, m_auxVectorField, dt);
	diffuse(m_velocityField, dt);
	project(m_velocityField);
}

void FluidSimulation::advect(VectorField2D& field, VectorField2D& auxField, float dt) {
	int width = field.width();
	int height = field.height();
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (isSolid(i, j)) { auxField.setValue(i, j, { 0.0f, 0.0f }); continue; }
			Vector2 position = { (float)i, (float)j };
			Vector2 currentVel = field.getValue(i, j);
			Vector2 newValue = field.sampleBilinear(position - currentVel * dt);
			auxField.setValue(i, j, newValue);
		}
	}
	std::swap(field, auxField);
}

void FluidSimulation::diffuse(VectorField2D& field, float dt) {

}

void FluidSimulation::project(VectorField2D& field) {

}
