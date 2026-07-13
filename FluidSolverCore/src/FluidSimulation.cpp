#include "FluidSimulation.h"

void FluidSimulation::step(float dt) {
	// All of the steps within the step function require an auxiliary vectorfield to which to write
	advect(m_velocityField, dt);
	diffuse(m_velocityField, dt);
	project(m_velocityField);
}

void FluidSimulation::advect(VectorField2D& field, float dt) {
	int width = field.width();
	int height = field.height();
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (isSolid(i, j)) { field.setValue(i, j, { 0.0f, 0.0f }); continue; }
			Vector2 position = { (float)i, (float)j };
			Vector2 currentVel = field.getValue(i, j);
			Vector2 newValue = field.sampleBilinear(position - currentVel * dt);
			field.setValue(i, j, newValue);
		}
	}
}

void FluidSimulation::diffuse(VectorField2D& field, float dt) {

}

void FluidSimulation::project(VectorField2D& field) {

}
