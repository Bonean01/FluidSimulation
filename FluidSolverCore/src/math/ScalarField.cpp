#include "math/ScalarField.h"

Vector2 ScalarField2D::gradient(int i, int j) const {
	float right = getValue(i + 1, j);
	float left = getValue(i - 1, j);
	float top = getValue(i, j + 1);
	float bottom = getValue(i, j - 1);
	float x = (right - left) / (2 * m_cellWidth);
	float y = (top - bottom) / (2 * m_cellWidth);
	return { x, y };
}

float ScalarField2D::laplacian(int i, int j) const {
	float center = getValue(i, j);
	float right = getValue(i + 1, j);
	float left = getValue(i - 1, j);
	float top = getValue(i, j + 1);
	float bottom = getValue(i, j - 1);
	return (right + left + top + bottom - 4 * center) / (m_cellWidth * m_cellWidth);
}
