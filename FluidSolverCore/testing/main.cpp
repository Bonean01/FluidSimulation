#include <iostream>
#include "FluidSimulation.h"
#include "math/dataStructures/VectorField.h"
#include "math/dataStructures/Vector.h"
#include "math/dataStructures/MACGrid.h"

//static void printSimulationState(const FluidSimulation& simulation) {
//	const VectorField2D& velField = simulation.getVelocityField();
//
//	int width = velField.width();
//	int height = velField.height();
//
//	for (int j = 0; j < height; j++) {
//		for (int i = 0; i < width; i++) {
//			Vec2f vec = simulation.getVelocity(i, j);
//			bool isSolid = simulation.isSolid(i, j) == 1;
//			std::cout << "(" << vec.x << ", " << vec.y << ")";
//			std::cout << (isSolid ? "@" : "-") << "\t";
//		}
//		std::cout << std::endl;
//	}
//}



static void printSimulationState(const FluidSimulation& simulation) {
	const MACGrid2D& velField = simulation.getVelocityField();

	int width = velField.width();
	int height = velField.height();

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Vec2f vec = simulation.getVelocity(i, j);
			bool isSolid = simulation.isSolid(i, j) == 1;
			std::cout << "(" << vec.x << ", " << vec.y << ")";
			std::cout << (isSolid ? "@" : "-") << "\t";
		}
		std::cout << std::endl;
	}
}


int main(int argc, char* argv[]) {
	int width = 5;
	int height = 5;
	FluidSimulation simulation{ width, height, 1.0f };
	for (int j = 0; j < height; j++) {
		simulation.setVelocity(3, j, { 0.0f, 10.0f });
	}

	printSimulationState(simulation);

	simulation.step(1.0f / 60);
	std::cout << "================== STEP TAKEN ===================" << std::endl;

	printSimulationState(simulation);
}
