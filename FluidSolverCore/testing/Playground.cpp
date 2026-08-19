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


#include "utils/profiling/Profiler.h"

int main(int argc, char* argv[]) {
	int width = 333;
	int height = 333;
	float cellWidth = 1.0f / width;
	float density = 1.0f;
	float kinematicViscosity = 0.001f;
	float timestep = 1.0f / 120.0f;

	FluidSimulation simulation{ width, height, cellWidth, density, kinematicViscosity };

	CellConfig movingWall{ {CellType::Solid}, {BoundaryCondition::Dirichlet, {1.0f, 0.0f}} };
	CellConfig staticWall{ {CellType::Solid}, {BoundaryCondition::Dirichlet, {0.0f, 0.0f}} };

	for (int k = 0; k < 10; k++)
		simulation.step(timestep);


	Profiler& profiler = Profiler::getInstance();

	Duration total{};
	for (auto& id : profiler.getIDs()) {
		Duration duration = profiler.getAverageDuration(id);
		total += duration;
		std::cout << id << ": " << duration << std::endl;
	}
	std::cout << "==============================" << std::endl;
	std::cout << "Total: " << total << std::endl;
}
