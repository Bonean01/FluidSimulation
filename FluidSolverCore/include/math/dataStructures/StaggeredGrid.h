#pragma once

#include "Vector.h"
#include <vector>

template<typename T>
class StaggeredGrid2D {
public:
    StaggeredGrid2D(int width, int height, float cellWidth = 1) :
        m_width(width),
        m_height(height),
        m_cellWidth(cellWidth),
        m_valuesX((width + 1) * height),
        m_valuesY(width * (height + 1)) {}

    int width() const { return m_width; }
    int height() const { return m_height; }
    float cellWidth() const { return m_cellWidth; }

    const T& getEdgeValue(const VectorComponent& C, int i, int j) const;

    void setEdgeValue(const VectorComponent& C, int i, int j, T value);
    
    int getValuesWidth(const VectorComponent& C) const;

    int getValuesHeight(const VectorComponent& C) const;

    Vec2f getEdgePosition(const VectorComponent& C, int i, int j) const;

protected:
    int m_width, m_height;
    float m_cellWidth;
    std::vector<T> m_valuesX, m_valuesY;
    

private:
    int getIndex(const VectorComponent& C, int i, int j) const;
};

#include "StaggeredGrid.tpp"
