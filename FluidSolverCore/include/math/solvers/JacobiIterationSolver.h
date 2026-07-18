#pragma once

#include "Grid.h"

class JacobiIterationSolver {
public:
	template<typename T>
	void solve(Grid2D<T>& newX, Grid2D<T>& x, Grid2D<T>& b, float alpha, float beta) const {
		int width = newX.width();
		int height = newX.height();
		if (x.width() != width || x.height() != height
		 || b.width() != width || b.height() != height) return;

		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				T right = x.getValue(i + 1, j);
				T left = x.getValue(i - 1, j);
				T top = x.getValue(i, j + 1);
				T bottom = x.getValue(i, j - 1);
				T valueB = b.getValue(i, j);
				T res = (right + left + top + bottom + alpha * valueB) / beta;
				newX.setValue(i, j, res);
			}
		}
	}


	void solve(ScalarField2D& newX, ScalarField2D& x, ScalarField2D& b, float alpha, float beta) const {
		int width = newX.width();
		int height = newX.height();
		if (x.width() != width || x.height() != height
			|| b.width() != width || b.height() != height) return;

		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				float right = x.getValue(i + 1, j);
				float left = x.getValue(i - 1, j);
				float top = x.getValue(i, j + 1);
				float bottom = x.getValue(i, j - 1);
				float valueB = b.getValue(i, j);
				float res = (right + left + top + bottom + alpha * valueB) / beta;
				newX.setValue(i, j, res);
			}
		}
	}


	void solveCell(int i, int j, ScalarField2D& newX, ScalarField2D& x, ScalarField2D& b, float alpha, float beta) {
		float right = x.getValue(i + 1, j);
		float left = x.getValue(i - 1, j);
		float top = x.getValue(i, j + 1);
		float bottom = x.getValue(i, j - 1);
		float valueB = b.getValue(i, j);
		float res = (right + left + top + bottom + alpha * valueB) / beta;
		newX.setValue(i, j, res);
	}
};
