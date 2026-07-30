#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <cstdint>

#include "math/dataStructures/MACGrid.h"
#include "steps/Advection.h"


using namespace Catch;

TEST_CASE("Advection - Constant velocity remains constant") {
	int width = 100;
	int height = 100;
	float cellWidth = 1.0f;
	float density = 1.0f;
	float timeStep = 1.0f / 60.0f;
	MACGrid2D velocityField{ width, height, cellWidth };
	Grid2D<uint8_t> solidCellMap{ width, height, cellWidth };
	const float CONSTANT = 10.0f;
	Advection advection{ width, height, cellWidth };

	// Initialize the velocity field to a constant value
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width + 1; i++) {
			velocityField.setEdgeX(i, j, CONSTANT);
		}
	}
	for (int j = 0; j < height + 1; j++) {
		for (int i = 0; i < width; i++) {
			velocityField.setEdgeY(i, j, CONSTANT);
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


	advection.execute(velocityField, solidCellMap, timeStep);


	// Check that all of the values remain the same
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width + 1; i++) {
			if (solidCellMap.getValue(i, j) || solidCellMap.getValue(i - 1, j)) continue;
			float edgeX = velocityField.getEdgeX(i, j);
			CHECK_THAT(edgeX, Matchers::WithinRel(CONSTANT));
		}
	}
	for (int j = 0; j < height + 1; j++) {
		for (int i = 0; i < width; i++) {
			if (solidCellMap.getValue(i, j) || solidCellMap.getValue(i, j - 1)) continue;
			float edgeY = velocityField.getEdgeY(i, j);
			CHECK_THAT(edgeY, Matchers::WithinRel(CONSTANT));
		}
	}
}
