#pragma once

#include <vector>
#include "Vector.h"
#include "Grid.h"
#include "ScalarField.h"


class VectorField2D : public Grid2D<Vec2f> {
public:
	VectorField2D(int width, int height, float cellWidth) : Grid2D(width, height, cellWidth) {}
	
	Vec2f sampleBilinear(float x, float y) const;
	Vec2f sampleBilinear(const Vec2f& position) const;
};
