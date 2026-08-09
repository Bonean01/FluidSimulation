#pragma once

#include "Vector.h"
#include <vector>

template<typename T>
class StaggeredGrid2D {
public:
    StaggeredGrid2D(int width, int height, float cellWidth = 1) :
        m_width(width),
        m_height(height),
        m_cellWidth(cellWidth) {}

    int width() { return m_width; }
    int height() { return m_height; }
    float cellWidth() { return m_cellWidth; }

    template<VectorComponent>
    T& getEdgeValue(int i, int j) const;

    template<VectorComponent>
    void setEdgeValue(int i, int j, T value);
    

protected:
    int m_width, m_height;
    float m_cellWidth;
    std::vector<T> m_valuesX, m_valuesY;
    

private:
    template<VectorComponent>
    int getIndex(int i, int j) const;
};

#include "StaggeredGrid.tpp"
