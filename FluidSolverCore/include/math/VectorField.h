#pragma once

#include <vector>
#include "math/Vector.h"

class VectorField2D {
public:
	const Vector2 sample(const Vector2& position) const;
	const float divergence(int i, int j) const;
	const float laplacian(int i, int j) const;
	int size() const { return m_size; }

	VectorField2D(int width, int height) :
		m_width(width),
		m_height(height),
		m_size(width * height),
		m_xComponent(m_size),
		m_yComponent(m_size) {}


private:
	int m_width, m_height, m_size;
	std::vector<float> m_xComponent;
	std::vector<float> m_yComponent;
	int getIndex(int i, int j) const { return j * m_width + i; }
};
