#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <cstdint>

#include "TestUtils.h"
#include "math/dataStructures/MACGrid.h"
#include "math/solvers/PressureSolver.h"
#include "math/operators/Staggered.h"
#include "steps/Projection.h"


using namespace Catch;

TEST_CASE("Projection - Divergence free field remains unchanged after projection") {
	int width = 100;
	int height = 100;
	float cellWidth = 1.0f;
	float density = 1.0f;
	float timeStep = 1.0f / 60.0f;
	int iterationCount = 60;
	MACGrid2D velocityField{ width, height, cellWidth };
	Grid2D<CellData> cellData{ width, height, cellWidth };
	ScalarField2D pressureField{ width, height, cellWidth };
	PressureSolver pressureSolver{ width, height, cellWidth };
	Projection projection{};

	TestUtils::initializeConstantVelocities(velocityField, 10.0f);

	pressureSolver.solveJacobi(pressureField, velocityField, cellData , density, timeStep, iterationCount);
	projection.execute(velocityField, pressureField, density, timeStep);
	
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			float divergence = std::abs(Staggered::divergence(i, j, velocityField));
			REQUIRE_THAT(divergence, Matchers::WithinAbs(0.0, std::numeric_limits<float>::epsilon()));
		}
	}
}
