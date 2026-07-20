#include <cmath>
#include <algorithm>
#include "math/dataStructures/VectorField.h"


Vec2f VectorField2D::sampleBilinear(float x, float y) const {
	x = std::clamp(x, 0.0f, (float)m_width - 2.0f);
	y = std::clamp(y, 0.0f, (float)m_height - 2.0f);

	int x1 = (int)std::floor(x);
	int x2 = x1 + 1;
	int y1 = (int)std::floor(y);
	int y2 = y1 + 1;
	float tX = x - x1;
	float tY = y - y1;

	Vec2f A = getValue(x1, y1);
	Vec2f B = getValue(x2, y1);
	Vec2f C = getValue(x1, y2);
	Vec2f D = getValue(x2, y2);
	
	return Vec2f::biLerp(A, B, C, D, tX, tY);
}

Vec2f VectorField2D::sampleBilinear(const Vec2f& position) const {
	return sampleBilinear(position.x, position.y);
}


float VectorField2D::divergence(int i, int j) const {
	Vec2f right = getValue(i + 1, j);
	Vec2f left = getValue(i - 1, j);
	Vec2f top = getValue(i, j + 1);
	Vec2f bottom = getValue(i, j - 1);
	return (right.x - left.x + top.y - bottom.y) / (2 * m_cellWidth);
}


Vec2f VectorField2D::laplacian(int i, int j) const {
	Vec2f center = getValue(i, j);
	Vec2f right = getValue(i + 1, j);
	Vec2f left = getValue(i - 1, j);
	Vec2f top = getValue(i, j + 1);
	Vec2f bottom = getValue(i, j - 1);
	return Vec2f {
		(right.x + left.x + top.x + bottom.x - 4 * center.x),
		(right.y + left.y + top.y + bottom.y - 4 * center.y)
	} / (m_cellWidth * m_cellWidth);
}
