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
	StaggeredVectorField2D field{ 5, 5, 0.5f };
	TestUtils::initializeRandomVelocities(field, -10.0f, 10.0f, 0);
	field.setEdgeValue<X>(1, 1, 5.0f);

	StaggeredVectorField2D laplacian{ 5, 5, 0.5f };
	Staggered::laplacian(laplacian, field);
	float laplacian1 = laplacian.getEdgeValue<X>(1, 1);
	std::cout << laplacian1 << std::endl;

	float right = field.getEdgeValue<X>(2, 1);
	float left = field.getEdgeValue<X>(0, 1);
	float top = field.getEdgeValue<X>(1, 2);
	float bottom = field.getEdgeValue<X>(1, 0);
	float center = field.getEdgeValue<X>(1, 1);
	float dx = field.cellWidth();

	float laplacian2 = (right + left + top + bottom - 4 * center) / (dx * dx);
	std::cout << laplacian2 << std::endl;
}
