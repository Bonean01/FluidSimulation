#include <iostream>
#include <cstdio>
#include "FluidSimulation.h"
#include "math/dataStructures/VectorField.h"
#include "math/dataStructures/Vector.h"
#include "math/dataStructures/MACGrid.h"


static void printSimulationState(const FluidSimulation& simulation) {
	const MACGrid2D& velField = simulation.getVelocityField();

	int width = velField.width();
	int height = velField.height();

	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			Vec2f vec = simulation.getVelocity(i, j);
			bool isSolid = simulation.isSolid(i, j) == 1;
			std::cout << "(" << vec.x << ", " << vec.y << ")";
			std::cout << (isSolid ? "@" : "-") << "\t\t";
		}
		std::cout << std::endl;
	}
}


static void printPressureField(FluidSimulation simulation) {
	ScalarField2D pressureField = simulation.getPressureField();
	int width = pressureField.width();
	int height = pressureField.height();
	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			std::cout << "(" << pressureField.getValue(i, j) << ")\t\t";
		}
		std::cout << std::endl;
	}
}


static void printDivergenceField(FluidSimulation simulation) {
	ScalarField2D divergenceField = simulation.getDivergenceField();
	int width = divergenceField.width();
	int height = divergenceField.height();
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			std::cout << "(" << divergenceField.getValue(i, j) << ")\t\t";
		}
		std::cout << std::endl;
	}
}


int main(int argc, char* argv[]) {
	int width = 5;
	int height = 5;
	FluidSimulation simulation{ width, height, 1.0f, 1.0f, 0.0f, 60 };
	for (int j = 0; j < height; j++) {
		simulation.setVelocity(1, j, { 10.0f, 0.0f });
	}

	printSimulationState(simulation);

	for (int i = 0; i < 1; i++)
		simulation.step(1.0f / 60);

	std::cout << "================== STEPS TAKEN ===================" << std::endl;

	std::cout << "PRESSURE FIELD" << std::endl;
	printPressureField(simulation);

	std::cout << "\nVELOCITY FIELD" << std::endl;
	printSimulationState(simulation);
}
