#include <iostream>

#include "TestUtils.h"

#include "FluidSimulation.h"
#include "math/dataStructures/VectorField.h"
#include "math/dataStructures/Vector.h"
#include "math/operators/Staggered.h"


static void printSimulationState(const FluidSimulation& simulation) {
	const StaggeredVectorField2D& velField = simulation.getVelocityField();

	int width = velField.width();
	int height = velField.height();

	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			Vec2f vec = simulation.getVelocity(i, j);
			bool isSolid = simulation.getCellData().getValue(i, j).cellType == CellType::Solid;
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
	using enum VectorComponent;

	FluidSimulation simulation{ 5, 5, 1.0f, 1.0f, 0.0f, 30 };

	simulation.setVelocity(0, 0, {10.0f, 10.0f});
	simulation.setCell(0, 0, { CellType::Solid }, { BoundaryCondition::Dirichlet, {0.0f, 0.0f} });

	Vec2f velAtEdge = simulation.getVelocity(0, 0);
	std::cout << velAtEdge.x << ", " << velAtEdge.y << std::endl;
	simulation.step(1.0f / 120.0f);
	velAtEdge = simulation.getVelocity(0, 0);
	std::cout << velAtEdge.x << ", " << velAtEdge.y << std::endl;
	std::cout << sizeof(CellData) << std::endl;
	std::cout << sizeof(BoundaryData) << std::endl;
}
