#pragma once

#include "StaggeredGrid.h"
#include <stdexcept>
#include <algorithm>


template<typename T>
template<VectorComponent C>
T& StaggeredGrid2D<T>::getEdgeValue(int i, int j) const {
    using enum VectorComponent;

    if constexpr (C == X) {
        i = std::clamp(i, 0, m_width);
        j = std::clamp(j, 0, m_height - 1);
        int index = getIndex<X>(i, j);

        return m_valuesX[index];
    }
    else if constexpr (C == Y) {
        i = std::clamp(i, 0, m_width - 1);
        j = std::clamp(j, 0, m_height);
        int index = getIndex<Y>(i, j);

        return m_valuesY[index];
    }
    else
        throw std::runtime_error(
            "StaggeredGrid2D only has components X and Y but " + C + "was accessed"
        );
}


template<typename T>
template<VectorComponent C>
void StaggeredGrid2D<T>::setEdgeValue(int i, int j, T value) {
    using enum VectorComponent;
    int index = getIndex<C>(i, j);

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
            "StaggeredGrid2D only has components X and Y but " + C + "was accessed"
        );
}


template<typename T>
template<VectorComponent C>
int StaggeredGrid2D<T>::getIndex(int i, int j) const {
    using enum VectorComponent;
    
    if constexpr (C == X)      return j * (m_width + 1) + i;
    else if constexpr (C == Y) return j * m_width + 1;
    else
        throw std::runtime_error(
            "StaggeredGrid2D only has components X and Y but " + C + "was accessed"
        );
}
