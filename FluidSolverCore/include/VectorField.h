#pragma once

#include <vector>

class VectorField2D {
public:
	// Bilinear interpolation is used to find values between cells
	const float valueAt(float x, float y);
	const float gradientAt(float x, float y);
	const float divergenceAt(float x, float y);

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
};
