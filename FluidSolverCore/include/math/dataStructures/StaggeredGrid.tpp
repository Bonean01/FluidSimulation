#pragma once

#include "StaggeredGrid.h"
#include <stdexcept>
#include <algorithm>


template<typename T>
const T& StaggeredGrid2D<T>::getEdgeValue(const VectorComponent& C, int i, int j) const {
    using enum VectorComponent;

    if (C == X) {
        i = std::clamp(i, 0, m_width);
        j = std::clamp(j, 0, m_height - 1);
        int index = getIndex(X, i, j);

        return m_valuesX[index];
    }
    else if (C == Y) {
        i = std::clamp(i, 0, m_width - 1);
        j = std::clamp(j, 0, m_height);
        int index = getIndex(Y, i, j);

        return m_valuesY[index];
    }
    else
        throw std::runtime_error(
            "StaggeredGrid2D only has components X and Y but accessed another one"
        );
}


template<typename T>
void StaggeredGrid2D<T>::setEdgeValue(const VectorComponent& C, int i, int j, T value) {
    using enum VectorComponent;
    int index = getIndex(C, i, j);

    if (C == X) {
	    if (index < 0 || index >= m_valuesX.size()) return;
	    m_valuesX[index] = value;
    }
    else if (C == Y) {
        if (index < 0 || index >= m_valuesY.size()) return;
        m_valuesY[index] = value;
    }
    else
        throw std::runtime_error(
            "StaggeredGrid2D only has components X and Y but accessed another one"
        );
}


template<typename T>
int StaggeredGrid2D<T>::getValuesWidth(const VectorComponent& C) const {
    using enum VectorComponent;

    if (C == X) return m_width + 1;
    else if (C == Y) return m_width;
    else
        throw std::runtime_error(
            "StaggeredGrid2D only has components X and Y but accessed another one"
        );
}


template<typename T>
int StaggeredGrid2D<T>::getValuesHeight(const VectorComponent& C) const {
    using enum VectorComponent;

    if (C == X) return m_height;
    else if (C == Y) return m_height + 1;
    else
        throw std::runtime_error(
            "StaggeredGrid2D only has components X and Y but accessed another one"
        );
}


template<typename T>
Vec2f StaggeredGrid2D<T>::getEdgePosition(const VectorComponent& C, int i, int j) const {
    using enum VectorComponent;

    if (C == X) return { (float)i - 0.5f, (float)j };
    else if (C == Y) return { (float)i, (float)j - 0.5f };
    else
        throw std::runtime_error(
            "StaggeredGrid2D only has components X and Y but accessed another one"
        );
}


template<typename T>
int StaggeredGrid2D<T>::getIndex(const VectorComponent& C, int i, int j) const {
    using enum VectorComponent;
    
    if (C == X)      return j * (m_width + 1) + i;
    else if (C == Y) return j * m_width + i;
    else
        throw std::runtime_error(
            "StaggeredGrid2D only has components X and Y but accessed another one"
        );
}
