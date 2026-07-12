#pragma once

#include <vector>

template<typename T>
class Grid2D {
public:
	Grid2D(int width, int height) :
		m_width(width),
		m_height(height),
		m_size(m_width * m_height)
		m_values(m_size) {}
	
	int width() const { return m_width; }
	int height() const { return m_height; }
	int size() const { return m_size; }

	void setValue(int i, int j, T value) {
		int index = getIndex(i, j);
		if (index >= m_size) throw std::exception("Index out of bounds for grid: (" + i + ", " + j + ")");
		m_values[index] = value;
	}

	T getValue(int i, int j) const {
		int index = getIndex(i, j);
		if (index >= m_size) throw std::exception("Index out of bounds for grid: (" + i + ", " + j + ")");
		return m_values[index];
	}


protected:
	int m_width, m_height, m_size;
	std::vector<T> m_values;


private:
	int getIndex(int i, int j) { return j * m_width + i; }
};
