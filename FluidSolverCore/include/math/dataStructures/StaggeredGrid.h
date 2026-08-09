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

    template<VectorComponent>
    const T& getEdgeValue(int i, int j) const;

    template<VectorComponent>
    void setEdgeValue(int i, int j, T value);
    
    template<VectorComponent>
    int getValuesWidth() const;

    template<VectorComponent>
    int getValuesHeight() const;

    template<VectorComponent>
    Vec2f getEdgePosition(int i, int j) const;

protected:
    int m_width, m_height;
    float m_cellWidth;
    std::vector<T> m_valuesX, m_valuesY;
    

private:
    template<VectorComponent>
    int getIndex(int i, int j) const;
};

#include "StaggeredGrid.tpp"
