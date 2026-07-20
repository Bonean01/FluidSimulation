#pragma once

#include <vector>
#include <algorithm>

template<typename T>
class Grid2D {
public:
	Grid2D(int width, int height, float cellWidth = 1) :
		m_width(width),
		m_height(height),
		m_cellWidth(cellWidth),
		m_cellCount(m_width * m_height),
		m_values(m_cellCount) {}
	
	int width() const { return m_width; }
	int height() const { return m_height; }
	float cellWidth() const { return m_cellWidth; }
	int cellCount() const { return m_cellCount; }

	void setValue(int i, int j, T value) {
		int index = getIndex(i, j);
		if (index < 0 || index >= m_cellCount) return;
		
		m_values[index] = value;
	}

	const T& getValue(int i, int j) const {
		i = std::clamp(i, 0, m_width - 1);
		j = std::clamp(j, 0, m_height - 1);
		int index = getIndex(i, j);

		return m_values[index];
	}

	const T* getValuesPtr() const {
		return m_values.data();
	}


protected:
	int m_width, m_height, m_cellCount;
	float m_cellWidth;
	std::vector<T> m_values;


private:
	int getIndex(int i, int j) const { return j * m_width + i; }
};
