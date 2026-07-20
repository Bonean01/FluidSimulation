#include <algorithm>
#include <cmath>
#include "math/dataStructures/MACGrid.h"


float MACGrid2D::getEdgeX(int i, int j) const {
	i = std::clamp(i, 0, m_width);
	j = std::clamp(j, 0, m_height - 1);
	int index = getIndexX(i, j);

	return m_edgeValuesX[index];
}


float MACGrid2D::getEdgeY(int i, int j) const {
	i = std::clamp(i, 0, m_width - 1);
	j = std::clamp(j, 0, m_height);
	int index = getIndexY(i, j);

	return m_edgeValuesY[index];
}


void MACGrid2D::setEdgeX(int i, int j, float value) {
	int index = getIndexX(i, j);
	if (index < 0 || index > m_cellWidth + m_width) return;

	m_edgeValuesX[index] = value;
}


void MACGrid2D::setEdgeY(int i, int j, float value) {
	int index = getIndexY(i, j);
	if (index < 0 || index > m_cellWidth + m_height) return;
	
	m_edgeValuesY[index] = value;
}


Vec2f MACGrid2D::getValue(int i, int j) const {
	float right = getEdgeX(i + 1, j);
	float left = getEdgeX(i, j);
	float top = getEdgeY(i, j + 1);
	float bottom = getEdgeY(i, j);
	float x = (right + left) / 2;
	float y = (top + bottom) / 2;
	return { x, y };
}

// Maybe we could abstract out VectorField out at let this class be an implementation of it
Vec2f MACGrid2D::sampleBilinear(float x, float y) const {
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


Vec2f MACGrid2D::sampleBilinear(const Vec2f& position) const {
	return sampleBilinear(position.x, position.y);
}


float MACGrid2D::divergence(int i, int j) const {
	float right = getEdgeX(i + 1, j);
	float left = getEdgeX(i, j);
	float top = getEdgeY(i, j + 1);
	float bottom = getEdgeY(i, j);
	return (right - left + top - bottom) / m_cellWidth;
}
