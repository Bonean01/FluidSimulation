#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <cstdint>

#include "TestUtils.h"
#include "steps/Advection.h"
#include "domain/BoundaryUtils.h"


using namespace Catch;

TEST_CASE("Advection - Constant velocity remains constant") {
	int width = 100;
	int height = 100;
	float cellWidth = 1.0f;
	float density = 1.0f;
	float timeStep = 1.0f / 60.0f;

	StaggeredVectorField2D velocityField{ width, height, cellWidth };
	StaggeredGrid2D<BoundaryData> boundaryData{ width, height, cellWidth };
	const float CONSTANT = 10.0f;

	Advection advection{ width, height, cellWidth };


	TestUtils::initializeConstantVelocities(velocityField, CONSTANT);

	advection.execute(velocityField, boundaryData, timeStep);

	// Check that all of the values remain the same
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width + 1; i++) {
			if (BoundaryUtils::hasBoundaryPrescribedVelocity(X, i, j, boundaryData)) continue;
			float edgeX = velocityField.getEdgeValue(X, i, j);
			CHECK_THAT(edgeX, Matchers::WithinRel(CONSTANT));
		}
	}
	for (int j = 0; j < height + 1; j++) {
		for (int i = 0; i < width; i++) {
			if (BoundaryUtils::hasBoundaryPrescribedVelocity(Y, i, j, boundaryData)) continue;
			float edgeY = velocityField.getEdgeValue(Y, i, j);
			CHECK_THAT(edgeY, Matchers::WithinRel(CONSTANT));
		}
	}
}
