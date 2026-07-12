#pragma once

#include <vector>

template<typename T>
class Grid2D {
public:
	Grid2D(int width, int height) :
		m_width(width),
		m_height(height),
		m_cellCount(m_width * m_height),
		m_values(m_cellCount) {}
	
	int width() const { return m_width; }
	int height() const { return m_height; }
	int size() const { return m_cellCount; }

	void setValue(int i, int j, T value) {
		int index = getIndex(i, j);
		if (i < 0 || i > m_width || j < 0 || j > m_height) throw std::exception("Index out of bounds for grid");
		m_values[index] = value;
	}

	const T& getValue(int i, int j) const {
		int index = getIndex(i, j);
		if (i < 0 || i > m_width || j < 0 || j > m_height) throw std::exception("Index out of bounds for grid");

		return m_values[index];
	}

	const T* getValuesPtr() const {
		return m_values.data();
	}


protected:
	int m_width, m_height, m_cellCount;
	std::vector<T> m_values;


private:
	int getIndex(int i, int j) const { return j * m_width + i; }
};
