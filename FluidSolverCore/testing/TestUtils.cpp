#include "TestUtils.h"



void TestUtils::initializeRandomVelocities(StaggeredVectorField2D& velocityField, float min, float max, unsigned int seed) {
	std::srand(seed);

	int width = velocityField.width();
	int height = velocityField.height();
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width + 1; i++) {
			float t = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
			float value = std::lerp(min, max, t);
			velocityField.setEdgeValue(VectorComponent::X, i, j, value);
		}
	}
	for (int j = 0; j < height + 1; j++) {
		for (int i = 0; i < width; i++) {
			float t = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
			float value = std::lerp(min, max, t);
			velocityField.setEdgeValue(VectorComponent::Y, i, j, value);
		}
	}
}


void TestUtils::initializeConstantVelocities(StaggeredVectorField2D& velocityField, float constant) {
	int width = velocityField.width();
	int height = velocityField.height();

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width + 1; i++) {
			velocityField.setEdgeValue(VectorComponent::X, i, j, constant);
		}
	}
	for (int j = 0; j < height + 1; j++) {
		for (int i = 0; i < width; i++) {
			velocityField.setEdgeValue(VectorComponent::Y, i, j, constant);
		}
	}
}


void TestUtils::initializeSolidBoundaries(Grid2D<uint8_t>& solidCellMap) {
	int width = solidCellMap.width();
	int height = solidCellMap.height();

	for (int i = 0; i < width; i++) {
		solidCellMap.setValue(i, 0, true);
		solidCellMap.setValue(i, height - 1, true);
	}
	for (int j = 0; j < height; j++) {
		solidCellMap.setValue(0, j, true);
		solidCellMap.setValue(width - 1, j, true);
	}
}


void TestUtils::initializeSolidBoundaries(Grid2D<CellData>& cellData) {
	int width = cellData.width();
	int height = cellData.height();
	CellData solidCell{ CellType::Solid };

	for (int i = 0; i < width; i++) {
		cellData.setValue(i, 0, solidCell);
		cellData.setValue(i, width - 1, solidCell);
	}
	for (int j = 0; j < height; j++) {
		cellData.setValue(j, 0, solidCell);
		cellData.setValue(j, height - 1, solidCell);
	}
}
