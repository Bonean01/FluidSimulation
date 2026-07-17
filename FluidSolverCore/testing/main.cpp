#include <iostream>
#include "FluidSimulation.h"
#include "math/dataStructures/VectorField.h"
#include "math/dataStructures/Vector.h"

static void printSimulationState(const FluidSimulation& simulation) {
	const VectorField2D& velField = simulation.getVelocityField();

	int width = velField.width();
	int height = velField.height();

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Vector2 vec = simulation.getVelocity(i, j);
			bool isSolid = simulation.isSolid(i, j) == 1;
			std::cout << "(" << vec.x << ", " << vec.y << ")";
			std::cout << (isSolid ? "@" : "-") << "\t";
		}
		std::cout << std::endl;
	}
}


int main(int argc, char* argv[]) {
	int width = 5; int height = 5;
	float fixedDeltaTime = 1.0f / 60.0f;
	FluidSimulation simulation{width, height, 1.0f};

	for (int j = 0; j < height; j++) {
		simulation.setVelocity(1, j, {10.0f, 0.0f});
	}

	printSimulationState(simulation);

	simulation.step(fixedDeltaTime);
	std::cout << "======================\nSTEP TAKEN\n======================"<< std::endl;

	printSimulationState(simulation);
}
