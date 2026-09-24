#pragma once

#include <queue>
#include <array>
#include <cstdint>

#include "math/dataStructures/Grid.h"
#include "math/dataStructures/ScalarField.h"
#include "domain/MarkerParticle.h"


struct SurfaceData {
	Vec2i position;
	Vec2f closestSurfacePointPos;
	float estimatedSD;
	bool known;
	bool inQueue;
	unsigned int depth;
};


class FluidSurface : public Grid2D<SurfaceData> {
public:
	FluidSurface(int width, int height) : Grid2D(width, height, 1), m_levelSet(width, height, 1) {
			m_container.reserve(m_cellCount * sizeof(QueueEntry));
			m_unknownsQueue = MinHeapPQ{ std::greater<QueueEntry>(), std::move(m_container) };
		}
	void update(const std::vector<MarkerParticle>& markerParticles, unsigned int depth);


private:
	struct QueueEntry {
		SurfaceData* surfaceData;

		auto operator <=>(const QueueEntry& other) const {
			return this->surfaceData->estimatedSD <=> other.surfaceData->estimatedSD;
		}
	};

	typedef std::priority_queue<QueueEntry, std::vector<QueueEntry>, std::greater<QueueEntry>> MinHeapPQ;

	ScalarField2D m_levelSet;
	std::vector<QueueEntry> m_container;
	MinHeapPQ m_unknownsQueue;
	std::array<Vec2i, 8> neighbourRelativePositions = {{
		{-1, 1}, {0, 1}, {1, 1}, {-1, 0}, {1, 0}, {-1, -1}, {0, -1}, {1, -1}
	}};

	void updateLevelSet(ScalarField2D& levelSet, const std::vector<MarkerParticle>& markerParticles);
	void resetSurfaceData();
	void setClosestCellsSD();
	void populateWithClosestNeighbours(MinHeapPQ& queue);
	void calculateSDF(unsigned int depth);


	std::array<SurfaceData*, 8> getNeighbours(const SurfaceData& current) { return getNeighbours(current.position.x, current.position.y); }
	std::array<SurfaceData*, 8> getNeighbours(int posX, int posY);
	std::array<float, 8> getNeighbourSignedDistances(int posX, int posY);
	unsigned int getNeighbourDepth(SurfaceData* neighbour);
	SurfaceData* extractNext(MinHeapPQ& queue);
	bool isInsideFluid(const SurfaceData& current);
};
