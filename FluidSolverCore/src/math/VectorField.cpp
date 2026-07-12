#include <cmath>
#include "math/VectorField.h"

const Vector2 VectorField2D::sample(const Vector2& position) const {
	float x = position.x;
	float y = position.y;

	if (x < 0 || x > m_width || y < 0 || y > m_height) throw std::exception("Value out of bounds of vector field");

	int x1 = std::floor(x);
	int x2 = std::ceil(x);
	int y1 = std::floor(y);
	int y2 = std::ceil(y);
	float tX = (x - x1) / (x2 - x1);
	float tY = (y - y1) / (y2 - y1);

	int indexA = getIndex(x1, y2);
	int indexB = getIndex(x2, y2);
	int indexC = getIndex(x1, y1);
	int indexD = getIndex(x2, y1);

	Vector2 A = { m_xComponent[indexA], m_yComponent[indexA] };
	Vector2 B = { m_xComponent[indexB], m_yComponent[indexB] };
	Vector2 C = { m_xComponent[indexC], m_yComponent[indexC] };
	Vector2 D = { m_xComponent[indexD], m_yComponent[indexD] };
	
	return Vector2::biLerp(A, B, C, D, tX, tY);
}


const float VectorField2D::divergence(int i, int j) const {
	return 0.0f;
}
