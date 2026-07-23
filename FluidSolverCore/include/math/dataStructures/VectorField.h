#pragma once

#include <vector>
#include "Vector.h"
#include "Grid.h"
#include "ScalarField.h"


class VectorField2D : public Grid2D<Vec2f> {
public:
	VectorField2D(int width, int height, float cellSeparation) : Grid2D(width, height, cellSeparation) {}
	
	Vec2f sampleBilinear(float x, float y) const;
	Vec2f sampleBilinear(const Vec2f& position) const;
	float divergence(int i, int j) const;
	void divergence(ScalarField2D& result) const;
	Vec2f laplacian(int i, int j) const;
};
