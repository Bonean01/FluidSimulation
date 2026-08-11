#include "math/dataStructures/StaggeredVectorField.h"

#include <cmath>
#include <algorithm>


Vec2f StaggeredVectorField2D::sampleBilinear(float x, float y) const {
        using enum VectorComponent;
        float resX = sampleBilinear(X, x, y);
        float resY = sampleBilinear(Y, x, y);
        return { resX, resY };
    }

    Vec2f StaggeredVectorField2D::sampleBilinear(const Vec2f& position) const {
        return sampleBilinear(position.x, position.y);
    }

	Vec2f StaggeredVectorField2D::getCellValue(int i, int j) const {
        using enum VectorComponent;
        float right = getEdgeValue(X, i + 1, j);
        float left = getEdgeValue(X, i, j);
        float top = getEdgeValue(Y, i, j + 1);
        float bottom = getEdgeValue(Y, i, j);
        float x = (right + left) / 2;
        float y = (top + bottom) / 2;
        return { x, y };
    }
    
	void StaggeredVectorField2D::setCellValue(int i, int j, Vec2f value) {
        using enum VectorComponent;
        float x = value.x;
        float y = value.y;
        setEdgeValue(X, i, j, x);
        setEdgeValue(X, i + 1, j, x);
        setEdgeValue(Y, i, j, y);
        setEdgeValue(Y, i, j + 1, y);
    }


    float StaggeredVectorField2D::sampleBilinear(const VectorComponent& C, float x, float y) const {
        using enum VectorComponent;

        if (C == X) {
            x = std::clamp(x, 0.0f, (float)m_width - 2.0f) + 0.5f;
            y = std::clamp(y, 0.0f, (float)m_height - 2.0f);
        }
        else if (C == Y) {
            x = std::clamp(x, 0.0f, (float)m_width - 2.0f);
            y = std::clamp(y, 0.0f, (float)m_height - 2.0f) + 0.5f;
        }

        int x1 = (int)std::floor(x);
        int x2 = x1 + 1;
        int y1 = (int)std::floor(y);
        int y2 = y1 + 1;

        float tX = x - x1;
        float tY = y - y1;

        float a = getEdgeValue(C, x1, y1);
        float b = getEdgeValue(C, x2, y1);
        float c = getEdgeValue(C, x1, y2);
        float d = getEdgeValue(C, x2, y2);

        float e = std::lerp(a, b, tX);
        float f = std::lerp(c, d, tX);

        return std::lerp(e, f, tY);
    }