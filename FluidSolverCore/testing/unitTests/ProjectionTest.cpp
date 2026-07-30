#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <cstdlib>
#include <cstdint>

#include "math/dataStructures/MACGrid.h"
#include "math/solvers/PressureSolver.h"
#include "math/operators/Staggered.h"
#include "steps/Projection.h"


using namespace Catch;

TEST_CASE("Projection - Zero velocity divergence after projection") {
	int width = 100;
	int height = 100;
	float cellWidth = 1.0f;
	float density = 1.0f;
	float timeStep = 1.0f / 60.0f;
	int iterationCount = 30;
	MACGrid2D velocityField{ width, height, cellWidth };
	Grid2D<uint8_t> solidCellMap{ width, height, cellWidth };
	ScalarField2D pressureField{ width, height, cellWidth };
	PressureSolver pressureSolver{ velocityField, solidCellMap };
	Projection projection{};

	// Initialize velocity field to random values between 0 and 1
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width + 1; i++) {
			float value = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
			velocityField.setEdgeX(i, j, value);
		}
	}
	for (int j = 0; j < height + 1; j++) {
		for (int i = 0; i < width; i++) {
			float value = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
			velocityField.setEdgeY(i, j, value);
		}
	}

	// Set the outer edges to be solid cells
	for (int i = 0; i < width; i++) {
		solidCellMap.setValue(i, 0, true);
		solidCellMap.setValue(i, height - 1, true);
	}
	for (int j = 0; j < height; j++) {
		solidCellMap.setValue(0, j, true);
		solidCellMap.setValue(width - 1, j, true);
	}


	pressureSolver.solveJacobi(pressureField, density, timeStep, iterationCount);
	projection.execute(velocityField, pressureField, solidCellMap, density, timeStep);
	

	// Compute the average divergence
	float totalDivergence = 0;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			totalDivergence += Staggered::divergence(i, j, velocityField);
		}
	}
	float averageDivergence = totalDivergence / static_cast<float>(width * height);

	REQUIRE_THAT(averageDivergence, Matchers::WithinAbs(0.0, 1e-8));
}
