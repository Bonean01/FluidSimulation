#include <algorithm>
#include <cmath>
#include <stdexcept>
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


float MACGrid2D::getEdge(const VectorComponent& component, int i, int j) const {
	if (component == VectorComponent::X) { return getEdgeX(i, j); }
	else if (component == VectorComponent::Y) { return getEdgeY(i, j); }
	else throw std::runtime_error("MACGrid2D only has X and Y components.");
}


void MACGrid2D::setEdgeX(int i, int j, float value) {
	int index = getIndexX(i, j);
	if (index < 0 || index >= m_edgeValuesX.size()) return;
	m_edgeValuesX[index] = value;
}


void MACGrid2D::setEdgeY(int i, int j, float value) {
	int index = getIndexY(i, j);
	if (index < 0 || index >= m_edgeValuesY.size()) return;
	
	m_edgeValuesY[index] = value;
}


void MACGrid2D::setEdge(const VectorComponent& component, int i, int j, float value) {
	if		(component == VectorComponent::X) { setEdgeX(i, j, value); }
	else if (component == VectorComponent::Y) { setEdgeY(i, j, value); }
}


Vec2f MACGrid2D::getCellValue(int i, int j) const {
	float right = getEdgeX(i + 1, j);
	float left = getEdgeX(i, j);
	float top = getEdgeY(i, j + 1);
	float bottom = getEdgeY(i, j);
	float x = (right + left) / 2;
	float y = (top + bottom) / 2;
	return { x, y };
}


void MACGrid2D::setCellValue(int i, int j, Vec2f value) {
	float x = value.x;
	float y = value.y;
	setEdgeX(i, j, x);
	setEdgeX(i + 1, j, x);
	setEdgeY(i, j, y);
	setEdgeY(i, j + 1, y);
}


Vec2f MACGrid2D::sampleBilinear(const Vec2f& position) const {
	return sampleBilinear(position.x, position.y);
}


Vec2f MACGrid2D::sampleBilinear(float x, float y) const {
	float resX = sampleBilinear(VectorComponent::X, x, y);
	float resY = sampleBilinear(VectorComponent::Y, x, y);
	return { resX, resY };
}


float MACGrid2D::sampleBilinear(const VectorComponent& component, float x, float y) const {
	if (component == VectorComponent::X) {
		x = std::clamp(x, 0.0f, (float)m_width - 2.0f) + 0.5f;
		y = std::clamp(y, 0.0f, (float)m_height - 2.0f);
	}
	else if (component == VectorComponent::Y) {
		x = std::clamp(x, 0.0f, (float)m_width - 2.0f);
		y = std::clamp(y, 0.0f, (float)m_height - 2.0f) + 0.5f;
	}

	int x1 = (int)std::floor(x);
	int x2 = x1 + 1;
	int y1 = (int)std::floor(y);
	int y2 = y1 + 1;

	float tX = x - x1;
	float tY = y - y1;

	float a = getEdge(component, x1, y1);
	float b = getEdge(component, x2, y1);
	float c = getEdge(component, x1, y2);
	float d = getEdge(component, x2, y2);

	float e = std::lerp(a, b, tX);
	float f = std::lerp(c, d, tX);

	return std::lerp(e, f, tY);
}
