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
	std::cout << "\n======================== FLUID NORMALS ========================" << std::endl;
	int width = fluidInterface.width();
	int height = fluidInterface.height();
	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			Vec2f vec = fluidInterface.getValue(i, j).normal;
			std::printf("(%.2f, %.2f)\t", vec.x, vec.y);
		}
		std::cout << std::endl;
	}

	std::cout << "\n======================== INTERCEPTS ========================\n";
	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			float intercept = fluidInterface.getValue(i, j).intercept;
			std::printf("(%.2f)\t", intercept);
		}
		std::cout << std::endl;
	}
}



#include "utils/profiling/Profiler.h"
#include "utils/profiling/ScopeProfiler.h"

#include "domain/PLIC/FluidInterface.h"

int main(int argc, char* argv[]) {
	int width = 5;
	int height = 5;
	float cellWidth = 1.0f;
	float density = 1.0f;
	float kinematicViscosity = 0.0001f;
	float timestep = 1.0f;

	Domain domain{ width, height };

	ScalarField2D volumeFractionField{ width, height, cellWidth };
	StaggeredVectorField2D velocityField{ width, height, cellWidth };
	StaggeredGrid2D<float> fluxes{ width, height, cellWidth }; 
	Advection advection{ width, height, cellWidth };

	volumeFractionField.setValue(1, 1, 1.0f);
	volumeFractionField.setValue(2, 1, 0.95f);
	volumeFractionField.setValue(3, 1, 0.05f);

	volumeFractionField.setValue(1, 2, 1.0f);
	volumeFractionField.setValue(2, 2, 0.8f);
	volumeFractionField.setValue(3, 2, 0.0f);

	volumeFractionField.setValue(1, 3, 0.9f);
	volumeFractionField.setValue(2, 3, 0.3f);
	volumeFractionField.setValue(3, 3, 0.0f);

	std::cout << "======================== VOLUME FRACTION FIELD ========================\n" << volumeFractionField;

	FluidInterface2D fluidInterface{ width, height };
	fluidInterface.reconstruct(volumeFractionField);
	printFluidInterface(fluidInterface);

	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height; j++) {
			velocityField.setEdgeValue(VectorComponent::Y, i, j, -0.125f);
		}
	}
	std::cout << "\n======================== VELOCITIES ========================\n" << velocityField;

	advection.computeFluxes(fluxes, fluidInterface, velocityField, volumeFractionField, timestep);

	std::cout << "\n======================== FLUXES ========================\n" << fluxes;
	domain.updateCellData(volumeFractionField);
	advection.execute(volumeFractionField, fluidInterface, velocityField, domain, timestep);
	std::cout << "\n======================== UPDATED VOLUME FRACTION FIELD ========================\n" << volumeFractionField;

	return 0;


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
