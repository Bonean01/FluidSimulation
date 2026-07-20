#pragma once

#include <vector>
#include "Vector.h"

class MACGrid2D {
public:
	MACGrid2D(int width, int height, float cellWidth) :
		m_width(width),
		m_height(height),
		m_cellWidth(cellWidth),
		m_edgeValuesX((m_width + 1) * m_height), 
		m_edgeValuesY(m_width * (m_height + 1)) {}

	int width() const { return m_width; }
	int height() const { return m_height; }
	float cellWidth() const { return m_cellWidth; }


	// SAMPLED AT CELL EDGE
	float getEdgeX(int i, int j) const;
	float getEdgeY(int i, int j) const;
	void setEdgeX(int i, int j, float value);
	void setEdgeY(int i, int j, float value);

	// Sampled at cell center
	Vec2f sampleBilinear(float i, float j) const;
	Vec2f sampleBilinear(const Vec2f& position) const;

	void setValue(int i, int j, Vec2f value);
	Vec2f getValue(int i, int j) const;

	float divergence(int i, int j) const;
	Vec2f laplacian(int i, int j) const;


private:
	int m_width, m_height;
	float m_cellWidth;
	std::vector<float> m_edgeValuesX, m_edgeValuesY;

	int getIndexX(int i, int j) const { return j * (m_width + 1) + i; }
	int getIndexY(int i, int j) const { return j * m_width + i; }
};
