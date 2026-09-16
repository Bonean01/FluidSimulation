#pragma once

#include <queue>
#include <array>
#include <cstdint>

#include "math/dataStructures/ScalarField.h"
#include "domain/MarkerParticle.h"


class FluidSurfaceSDF_2D : public ScalarField2D {
public:
	FluidSurfaceSDF_2D(int width, int height, float cellWidth) :
		ScalarField2D(width, height, cellWidth),
		m_levelSet(width, height, cellWidth),
		m_SDFCellData(width, height, cellWidth) {
	}

	void update(const std::vector<MarkerParticle>& markerParticles, unsigned int depth);


private:
	struct SDFCellData {
		Vec2f closestSurfacePointPos;
		float estimatedSD;
		bool known;

		friend bool operator >(const SDFCellData& lhs, const SDFCellData& rhs) {
			return lhs.estimatedSD > rhs.estimatedSD;
		}
	};

	ScalarField2D m_levelSet;
	typedef std::priority_queue<SDFCellData, std::vector<SDFCellData>, std::greater<SDFCellData>> MinHeapPQ;
	MinHeapPQ m_unknownsQueue;
	Grid2D<SDFCellData> m_SDFCellData;

	void updateLevelSet(ScalarField2D& levelSet, const std::vector<MarkerParticle>& markerParticles);
	void calculateSDF(unsigned int depth);

	std::array<SDFCellData*, 8> getNeighbours(int posX, int posY) {
		auto neighbours = std::array<SDFCellData*, 8>();
		int width = m_SDFCellData.width();
		int height = m_SDFCellData.height();

		constexpr std::array<std::pair<int, int>, 8> pairs = 
			{{{-1, 1}, {0, 1}, {1, 1}, {-1, 0}, {1, 0}, {-1, -1}, {0, -1}, {1, -1}}};

		auto res = std::array<SDFCellData*, 8>{};
		int k = 0;
		for (auto& pairs : pairs) {
			int i = posX + pairs.first;
			int j = posY + pairs.second;

			if (0 >= i && i < width && 0 >= j && j < height) {
				res[k] = &m_SDFCellData.at(i, j);
			}
			k++;
		}
		return res;
	}
};
