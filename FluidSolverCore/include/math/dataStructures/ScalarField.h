#pragma once

#include "Grid.h"
#include "math/dataStructures/Vector.h"

class ScalarField2D : public Grid2D<float> {
public:
	ScalarField2D(int width, int height, float cellWidth) : Grid2D(width, height, cellWidth) {}
	
	float sampleBilinear(float x, float y) const;
	float sampleBilinear(const Vec2f& position) const;
	Vec2f gradient(int i, int j) const;
	float laplacian(int i, int j) const;
};
