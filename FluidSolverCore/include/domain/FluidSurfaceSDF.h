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
		m_SDFCellData(width, height, cellWidth) { }

	void update(const std::vector<MarkerParticle>& markerParticles, unsigned int depth);


private:
	struct SDFCellData {
		Vec2i position;
		Vec2f closestSurfacePointPos;
		float estimatedSD;
		bool known;
		unsigned int depth;
	};

	struct QueueEntry {
		Vec2i position;
		float priority;

		auto operator <=>(const QueueEntry& other) const {
			return this->priority <=> other.priority;
		}
	};

	ScalarField2D m_levelSet;
	typedef std::priority_queue<QueueEntry, std::vector<QueueEntry>, std::greater<QueueEntry>> MinHeapPQ;
	MinHeapPQ m_unknownsQueue;
	Grid2D<SDFCellData> m_SDFCellData;

	std::array<Vec2i, 8> neighbourRelativePositions = {{
		{-1, 1}, {0, 1}, {1, 1}, {-1, 0}, {1, 0}, {-1, -1}, {0, -1}, {1, -1}
	}};

	void updateLevelSet(ScalarField2D& levelSet, const std::vector<MarkerParticle>& markerParticles);
	void calculateSDF(unsigned int depth);


	std::array<SDFCellData*, 8> getNeighbours(int posX, int posY) {
		int width = m_SDFCellData.width();
		int height = m_SDFCellData.height();

		auto res = std::array<SDFCellData*, 8>{};
		for (int k = 0; k < neighbourRelativePositions.size(); k++) {
			auto& neighbourRelativePos = neighbourRelativePositions[k];
			int i = posX + neighbourRelativePos.x;
			int j = posY + neighbourRelativePos.y;

			if (0 <= i && i < width && 0 <= j && j < height) {
				res[k] = &m_SDFCellData.at(i, j);
			}
		}
		return res;
	}


	std::array<float, 8> getNeighbourSignedDistances(int i, int j) {
		auto res = std::array<float, 8>{};
		for (int i = 0; i < res.size(); i++) { res[i] = std::numeric_limits<float>::infinity(); }
		
		auto neighbours = getNeighbours(i, j);
		float currentLS = m_levelSet.getValue(i, j);

		for (int k = 0; k < neighbours.size(); k++) {
			auto& neighbour = neighbours[k];
			auto& neighbourRelativePos = neighbourRelativePositions[k];
			float neighbourLS = m_levelSet.getValue(i + neighbourRelativePos.x, j + neighbourRelativePos.y);
			
			if (std::signbit(neighbourLS) != std::signbit(currentLS)) {
				float t = currentLS / (currentLS - neighbourLS);
				float distance = t * neighbourRelativePos.magnitude();
				float signedDistance = currentLS < 0 ? -distance : distance;
				res[k] = signedDistance;
			}
		}
		return res;
	}
};
