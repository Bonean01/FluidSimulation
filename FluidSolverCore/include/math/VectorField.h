#pragma once

#include <vector>
#include "math/Vector.h"
#include "Grid.h"

class VectorField2D : public Grid2D<Vector2> {
public:
	VectorField2D(int width, int height) : Grid2D(width, height) {}
	
	Vector2 sampleBilinear(float x, float y) const;
	Vector2 sampleBilinear(const Vector2& position) const;
	float divergence(int i, int j) const;
	float laplacian(int i, int j) const;
};
