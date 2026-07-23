#include <cmath>
#include "math/dataStructures/ScalarField.h"

float ScalarField2D::sampleBilinear(float x, float y) const {
	x = std::clamp(x, 0.0f, (float)m_width - 2.0f);
	y = std::clamp(y, 0.0f, (float)m_height - 2.0f);

	int x1 = (int)std::floor(x);
	int x2 = x1 + 1;
	int y1 = (int)std::floor(y);
	int y2 = y1 + 1;
	float tX = x - x1;
	float tY = y - y1;

	float A = getValue(x1, y1);
	float B = getValue(x2, y1);
	float C = getValue(x1, y2);
	float D = getValue(x2, y2);

	float E = std::lerp(A, B, tX);
	float F = std::lerp(C, D, tX);

	return std::lerp(E, F, tY);
}


float ScalarField2D::sampleBilinear(const Vec2f& position) const {
	return sampleBilinear(position.x, position.y);
}


Vec2f ScalarField2D::gradient(int i, int j) const {
	float center = getValue(i, j);
	float right = getValue(i + 1, j);
	float top = getValue(i, j + 1);
	return Vec2f{ center - right, center - top } / m_cellWidth;
}

float ScalarField2D::laplacian(int i, int j) const {
	float center = getValue(i, j);
	float right = getValue(i + 1, j);
	float left = getValue(i - 1, j);
	float top = getValue(i, j + 1);
	float bottom = getValue(i, j - 1);
	return (right + left + top + bottom - 4 * center) / (m_cellWidth * m_cellWidth);
}
