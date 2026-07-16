#include <cmath>
#include <algorithm>
#include <iostream>
#include "math/VectorField.h"

#include <iostream>

Vector2 VectorField2D::sampleBilinear(float x, float y) const {
	x = std::clamp(x, 0.0f, (float)m_width - 2.0f);
	y = std::clamp(y, 0.0f, (float)m_height - 2.0f);

	int x1 = (int)std::floor(x);
	int x2 = x1 + 1;
	int y1 = (int)std::floor(y);
	int y2 = y1 + 1;
	float tX = x - x1;
	float tY = y - y1;

	Vector2 A = getValue(x1, y1);
	Vector2 B = getValue(x2, y1);
	Vector2 C = getValue(x1, y2);
	Vector2 D = getValue(x2, y2);
	
	return Vector2::biLerp(A, B, C, D, tX, tY);
}

Vector2 VectorField2D::sampleBilinear(const Vector2& position) const {
	return sampleBilinear(position.x, position.y);
}


float VectorField2D::divergence(int i, int j) const {
	Vector2 right = getValue(i + 1, j);
	Vector2 left = getValue(i - 1, j);
	Vector2 top = getValue(i, j + 1);
	Vector2 bottom = getValue(i, j - 1);
	return (right.x - left.x + top.y - bottom.y) / (2 * m_cellWidth);
}


Vector2 VectorField2D::laplacian(int i, int j) const {
	Vector2 center = getValue(i, j);
	Vector2 right = getValue(i + 1, j);
	Vector2 left = getValue(i - 1, j);
	Vector2 top = getValue(i, j + 1);
	Vector2 bottom = getValue(i, j - 1);
	return Vector2 {
		(right.x + left.x + top.x + bottom.x - 4 * center.x),
		(right.y + left.y + top.y + bottom.y - 4 * center.y)
	} / (m_cellWidth * m_cellWidth);
}
