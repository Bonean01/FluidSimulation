#pragma once

#include "StaggeredGrid.h"
#include <cmath>

class StaggeredVectorField2D : public StaggeredGrid2D<float> {
public:
    StaggeredVectorField2D(int width, int height, float cellWidth) : StaggeredGrid2D(width, height, cellWidth) {}
    
    Vec2f sampleBilinear(float x, float y) const {
        using enum VectorComponent;
        float resX = sampleBilinear<X>(x, y);
        float resY = sampleBilinear<Y>(x, y);
        return { resX, resY };
    }

    Vec2f sampleBilinear(const Vec2f& position) const {
        return sampleBilinear(position.x, position.y);
    }

	Vec2f getCellValue(int i, int j) const {
        return sampleBilinear(i + 0.5f, j + 0.5f);
    }
    
	void setCellValue(int i, int j, Vec2f value) {
        float x = value.x;
        float y = value.y;
        setEdgeValue<X>(i, j, x);
        setEdgeValue<X>(i + 1, j, x);
        setEdgeValue<Y>(i, j, y);
        setEdgeValue<Y>(i, j + 1, y);
    }


private:
    template<VectorComponent C>
    float sampleBilinear(float x, float y) const {
        using enum VectorComponent;

        if constexpr (C == X) {
            x = std::clamp(x, 0.0f, (float)m_width - 2.0f) + 0.5f;
            y = std::clamp(y, 0.0f, (float)m_height - 2.0f);
        }
        else if constexpr (C == Y) {
            x = std::clamp(x, 0.0f, (float)m_width - 2.0f);
            y = std::clamp(y, 0.0f, (float)m_height - 2.0f) + 0.5f;
        }

        int x1 = (int)std::floor(x);
        int x2 = x1 + 1;
        int y1 = (int)std::floor(y);
        int y2 = y1 + 1;

        float tX = x - x1;
        float tY = y - y1;

        float a = getEdgeValue<C>(x1, y1);
        float b = getEdgeValue<C>(x2, y1);
        float c = getEdgeValue<C>(x1, y2);
        float d = getEdgeValue<C>(x2, y2);

        float e = std::lerp(a, b, tX);
        float f = std::lerp(c, d, tX);

        return std::lerp(e, f, tY);
    }
};
