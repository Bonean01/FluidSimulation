#include "math/dataStructures/ScalarField.h"

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
