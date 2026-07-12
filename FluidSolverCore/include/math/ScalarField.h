#pragma once

#include "Grid.h"
#include "math/Vector.h"

class ScalarField2D : public Grid2D<float> {
public:
	ScalarField2D(int width, int height) : Grid2D(width, height) {}
	
	Vector2 gradient(int i, int j) const;
	float laplacian(int i, int j) const;
};
