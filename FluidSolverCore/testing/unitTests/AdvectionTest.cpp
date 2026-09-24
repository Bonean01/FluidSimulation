#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <cstdint>

#include "TestUtils.h"
#include "steps/Advection.h"
#include "domain/Domain.h"


using namespace Catch;

TEST_CASE("Advection - Constant velocity remains constant") {
	int width = 100;
	int height = 100;
	float cellWidth = 1.0f;
	float density = 1.0f;
	float timeStep = 1.0f / 60.0f;

	StaggeredVectorField2D velocityField{ width, height, cellWidth };
	Domain domain{ width, height };
	const float CONSTANT = 10.0f;

	Advection advection{ width, height, cellWidth };


	TestUtils::initializeConstantVelocities(velocityField, CONSTANT);

	advection.execute(velocityField, domain, timeStep);

	// Check that all of the values remain the same
	std::array<VectorComponent, 2> components = { VectorComponent::X, VectorComponent::Y };
	for (VectorComponent C : components) {
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width + 1; i++) {
				if (domain.hasEdgePrescribedVelocity(C, i, j)) continue;
				float edge = velocityField.getEdgeValue(C, i, j);
				CHECK_THAT(edge, Matchers::WithinRel(CONSTANT));
			}
		}
	}
}
