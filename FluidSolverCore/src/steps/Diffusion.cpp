#include "steps/Diffusion.h"


void Diffusion::execute(MACGrid2D& velocityField, float timeStep) {
	int width = velocityField.width();
	int height = velocityField.height();

	// === horizontal ===
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width + 1; i++) {

		}
	}
	// === vertical ===
	for (int j = 0; j < height + 1; j++) {
		for (int i = 0; i < width; i++) {

		}
	}
}
