#include <cmath>
#include "math/VectorField.h"

Vector2 VectorField2D::sampleBilinear(float x, float y) const {
	if (x < 0 || x > m_width || y < 0 || y > m_height) throw std::exception("Value out of bounds of vector field");

	int x1 = std::floor(x);
	int x2 = std::ceil(x);
	int y1 = std::floor(y);
	int y2 = std::ceil(y);
	float tX = (x - x1) / (x2 - x1);
	float tY = (y - y1) / (y2 - y1);

	Vector2 A = getValue(x1, y2);
	Vector2 B = getValue(x2, y2);
	Vector2 C = getValue(x1, y1);
	Vector2 D = getValue(x2, y1);
	
	return Vector2::biLerp(A, B, C, D, tX, tY);
}

Vector2 VectorField2D::sampleBilinear(const Vector2& position) const {
	return sampleBilinear(position.x, position.y);
}


float VectorField2D::divergence(int i, int j) const {
	return 0.0f;
}
