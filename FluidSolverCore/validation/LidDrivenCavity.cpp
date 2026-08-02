#include "FluidSimulation.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include "TestUtils.h"


int main(int argc, char* argv[]) {
	int width = 33;
	int height = width;
	float cellWidth = 1.0f / width;
	float density = 1.0f;
	float kinematicViscosity = 1e-05f;
	float lidSpeed = 1.0f;
	unsigned int solverIterationCount = 30;
	unsigned int simulationIterationCount = 1000;
	float timeStep = 1.0f / 120;

	std::cout << "Initializing..." << std::endl;

	FluidSimulation simulation{
		width,
		height,
		cellWidth,
		density,
		kinematicViscosity,
		solverIterationCount
	};
	
	CellData wallData = { CellType::Solid, BoundaryType::NoSlip, {0.0f, 0.0f} };
	CellData lidData = { CellType::Solid, BoundaryType::NoSlip, {lidSpeed, 0.0f} };
	for (int i = 0; i < width; i++) {
		simulation.setCellData(i, 0, wallData);
		simulation.setCellData(i, height - 1, lidData);
	}
	for (int j = 0; j < height - 1; j++) {
		simulation.setCellData(0, j, wallData);
		simulation.setCellData(width - 1, j, wallData);
	}

	
	std::cout << "Running..." << std::endl;
	for (unsigned int k = 0; k < simulationIterationCount; k++) {
		simulation.step(timeStep);
		std::cout << k << std::endl;
	}


	std::cout << "Writting results..." << std::endl;
	std::filesystem::create_directory("Results");
	std::ofstream resultsFile{"Results/LidDrivenCavity.csv"};

	const MACGrid2D& velocityField = simulation.getVelocityField();
	const ScalarField2D& pressureField = simulation.getPressureField();

	resultsFile << "u, v, p" << std::endl;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Vec2f vel = simulation.getVelocity(i, j);
			float u = vel.x;
			float v = vel.y;
			float p = pressureField.getValue(i, j);
			resultsFile << u << ", " << v << ", " << p << std::endl;
		}
	}
	resultsFile.close();

	std::cout << "Done!" << std::endl;
}
