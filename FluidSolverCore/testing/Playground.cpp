#include <iostream>
#include <cstdio>

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


static void printPressureField(const FluidSimulation& simulation) {
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


static void printDivergenceField(const FluidSimulation& simulation) {
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


static void printSurfaceSDF(const FluidSimulation& simulation) {
	FluidSurface surfaceData = simulation.getSurfaceSDF();
	int width = surfaceData.width();
	int height = surfaceData.height();
	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			float value = surfaceData.getValue(i, j);
			float roundedValue = static_cast<int>(value * 100) / 100.0f;
			std::cout << "(" << (value == std::numeric_limits<float>::infinity() ? value : roundedValue) << ")\t\t";
		}
		std::cout << std::endl;
	}
}


static void printFluidInterface(const FluidInterface2D& fluidInterface) {
	int width = fluidInterface.width();
	int height = fluidInterface.height();
	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			Vec2f vec = fluidInterface.getValue(i, j).normal;
			std::printf("(%.2f, %.2f)\t", vec.x, vec.y);
		}
		std::cout << std::endl;
	}

	std::cout << "\nINTERCEPTS" << std::endl;
	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			float intercept = fluidInterface.getValue(i, j).intercept;
			std::printf("(%.2f)\t", intercept);
		}
		std::cout << std::endl;
	}
}


static void printScalarField(const ScalarField2D& scalarField) {
	int width = scalarField.width();
	int height = scalarField.height();
	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			float value = scalarField.getValue(i, j);
			std::printf("(%.2f)\t", value);
		}
		std::cout << std::endl;
	}
}


#include "utils/profiling/Profiler.h"
#include "utils/profiling/ScopeProfiler.h"

#include "domain/PLIC/FluidInterface.h"
#include "domain/PLIC/InterfaceConstructor.h"

int main(int argc, char* argv[]) {
	ScalarField2D volumeFractionField{ 3, 3, 1.0f };

	volumeFractionField.setValue(0, 0, 1.0f);
	volumeFractionField.setValue(1, 0, 1.0f);
	volumeFractionField.setValue(2, 0, 0.95f);

	volumeFractionField.setValue(0, 1, 1.0f);
	volumeFractionField.setValue(1, 1, 0.9f);
	volumeFractionField.setValue(2, 1, 0.3f);

	volumeFractionField.setValue(0, 2, 0.7f);
	volumeFractionField.setValue(1, 2, 0.15f);
	volumeFractionField.setValue(2, 2, 0.0f);


	std::cout << "VOLUME FRACTION FIELD" << std::endl;
	printScalarField(volumeFractionField);

	InterfaceConstructor interfaceConstructor{};

	const FluidInterface2D& fluidInterface = interfaceConstructor.construct(volumeFractionField);

	std::cout << "\nFLUID NORMALS" << std::endl;
	printFluidInterface(fluidInterface);


	return 0;






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

	simulation.createMarkerParticles();
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
