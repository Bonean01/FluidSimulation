#include <iostream>
#include "FluidSimulation.h"
#include "math/dataStructures/VectorField.h"
#include "math/dataStructures/Vector.h"
#include "math/dataStructures/MACGrid.h"
#include "math/operators/Staggered.h"


static void printSimulationState(const FluidSimulation& simulation) {
	const MACGrid2D& velField = simulation.getVelocityField();

	int width = velField.width();
	int height = velField.height();

	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			Vec2f vec = simulation.getVelocity(i, j);
			bool isSolid = simulation.getSolidCellMap().getValue(i, j);
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
	MACGrid2D field{ 5, 5, 1.0f };
	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < 6; i++) {
			if (i == 1 && j == 1) { field.setEdgeX(i, j, 5.0f); continue; }
			field.setEdgeX(i, j, 1.0f);
		}
	}
	
	for (int j = 0; j < 6; j++) {
		for (int i = 0; i < 5; i++) {
			field.setEdgeY(i, j, 1.0f);
		}
	}

	MACGrid2D laplacian{ 5, 5, 1.0f };
	Staggered::laplacian(laplacian, field);
	std::cout << laplacian.getEdgeX(1, 1) << std::endl;
}
