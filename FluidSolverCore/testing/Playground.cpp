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
			float roundedX = static_cast<int>(vec.x * 100) / 100.0f;
			float roundedY = static_cast<int>(vec.y * 100) / 100.0f;
			bool isSolid = simulation.getCellData().getValue(i, j).cellType == CellType::Solid;
			bool isFluid = simulation.getCellData().getValue(i, j).cellType == CellType::Fluid;
			std::cout << "(" << roundedX << ", " << roundedY << ")";
			std::cout << (isSolid ? "@" : isFluid ? "O" : "-") << "\t";
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


static void printSurfaceSDF(FluidSimulation simulation) {
	FluidSurface surfaceData = simulation.getSurfaceSDF();
	int width = surfaceData.width();
	int height = surfaceData.height();
	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			float value = surfaceData.getValue(i, j).estimatedSD;
			float roundedValue = static_cast<int>(value * 100) / 100.0f;
			std::cout << "(" << (value == std::numeric_limits<float>::infinity() ? value : roundedValue) << ")\t\t";
		}
		std::cout << std::endl;
	}
}


#include "utils/profiling/Profiler.h"
#include "utils/profiling/ScopeProfiler.h"

int main(int argc, char* argv[]) {
	int width = 88;
	int height = 44;
	float cellWidth = 0.75f; //1.0f / width;
	float density = 1.0f;
	float kinematicViscosity = 0.0001f;
	float timestep = 1.0f / 30.0f;

	FluidSimulation simulation{ 
		FluidSimulationConfig{ 
			width, 
			height, 
			cellWidth, 
			density, 
			kinematicViscosity,
			LinearSolverConfig{
				LinearSolverAlgorithm::JACOBI,
				30
			},
			20,
			true
		}
	};

	CellConfig inlet{ {CellType::Fluid}, {BoundaryCondition::Dirichlet, {1.0f, 0.0f}} };
	CellConfig outflow{ {CellType::Fluid}, {BoundaryCondition::HomogeneousNeumann} };
	CellConfig staticWall{ {CellType::Solid}, {BoundaryCondition::Dirichlet, {0.0f, 0.0f}} };
	CellConfig fluid{ {CellType::Fluid}, {BoundaryCondition::None} };


	for (int j = 10; j < height - 10; j++) {
		for (int i = 10; i < width - 10; i++) {
			simulation.setCell(i, j, fluid);
		}
	}

	for (int j = 0; j < height; j++) {
		simulation.setCell(0, j, staticWall);
		simulation.setCell(width - 1, j, staticWall);
	}

	for (int i = 0; i < width; i++) {
		simulation.setCell(i, 0, staticWall);
		simulation.setCell(i, height - 1, staticWall);
	}

	for (int k = 0; k < 1; k++) {
		simulation.step(timestep);
	}
	//printSimulationState(simulation);

	Profiler& profiler = Profiler::getInstance();

	for (auto& id : profiler.getIDs()) {
		Duration duration = profiler.getTaskAverageDuration(id);
		std::cout << id << ": " << duration << std::endl;
	}
}
