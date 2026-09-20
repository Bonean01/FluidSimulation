#pragma once

#include <queue>
#include <array>
#include <cstdint>

//temp
#include <iostream>
#include <cstdio>

#include "math/dataStructures/Grid.h"
#include "math/dataStructures/VectorField.h"
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
	FluidSurface(int width, int height) :
		Grid2D(width, height, 1),
		m_levelSet(width, height, 1) {
			m_container.reserve(m_cellCount * sizeof(QueueEntry));
			m_unknownsQueue = MinHeapPQ{ std::greater<QueueEntry>(), std::move(m_container) };
		}

	void update(const std::vector<MarkerParticle>& markerParticles, unsigned int depth);

	Vec2f gradient(int i, int j) const {
		const SurfaceData& current = this->getValue(i, j);
		Vec2f distance = (current.closestSurfacePointPos - static_cast<Vec2f>(current.position));
		distance.normalize();
		return distance;
	}

private:
	struct QueueEntry {
		SurfaceData* surfaceData;

		auto operator <=>(const QueueEntry& other) const {
			return this->surfaceData->estimatedSD <=> other.surfaceData->estimatedSD;
		}
	};

	ScalarField2D m_levelSet;

	std::vector<QueueEntry> m_container;
	typedef std::priority_queue<QueueEntry, std::vector<QueueEntry>, std::greater<QueueEntry>> MinHeapPQ;
	MinHeapPQ m_unknownsQueue;

	std::array<Vec2i, 8> neighbourRelativePositions = {{
		{-1, 1}, {0, 1}, {1, 1}, {-1, 0}, {1, 0}, {-1, -1}, {0, -1}, {1, -1}
	}};

	void updateLevelSet(ScalarField2D& levelSet, const std::vector<MarkerParticle>& markerParticles);
	void calculateSDF(unsigned int depth);


	std::array<SurfaceData*, 8> getNeighbours(const SurfaceData& current) {
		return getNeighbours(current.position.x, current.position.y);
	}

	std::array<SurfaceData*, 8> getNeighbours(int posX, int posY) {
		auto res = std::array<SurfaceData*, 8>{};
		for (int k = 0; k < neighbourRelativePositions.size(); k++) {
			auto& neighbourRelativePos = neighbourRelativePositions[k];
			int i = posX + neighbourRelativePos.x;
			int j = posY + neighbourRelativePos.y;

			if (0 <= i && i < m_width && 0 <= j && j < m_height) {
				res[k] = &this->at(i, j);
			}
		}
		return res;
	}


	std::array<float, 8> getNeighbourSignedDistances(int posX, int posY) {
		auto res = std::array<float, 8>{};
		for (int i = 0; i < res.size(); i++) { res[i] = std::numeric_limits<float>::infinity(); }
		
		auto neighbours = getNeighbours(posX, posY);
		float currentLS = m_levelSet.getValue(posX, posY);

		for (int k = 0; k < neighbours.size(); k++) {
			auto& neighbour = neighbours[k];
			auto& neighbourRelativePos = neighbourRelativePositions[k];
			float neighbourLS = m_levelSet.getValue(posX + neighbourRelativePos.x, posY + neighbourRelativePos.y);
			
			if (std::signbit(neighbourLS) != std::signbit(currentLS)) {
				float t = currentLS / (currentLS - neighbourLS);
				float distance = t * neighbourRelativePos.magnitude();
				float signedDistance = currentLS < 0 ? -distance : distance;
				res[k] = signedDistance;
			}
		}
		return res;
	}


	bool isInsideFluid(const SurfaceData& current) {
		int posX = current.position.x;
		int posY = current.position.y;

		return m_levelSet.getValue(posX, posY) < 0;
	}


public:
	//temp
	void printClosestSurfacePoints() const {
		for (int j = m_height - 1; j >= 0; j--) {
			for (int i = 0; i < m_width; i++) {
				Vec2f closestSurfacePoint = this->getValue(i, j).closestSurfacePointPos;
				std::printf("(%.2f, %.2f)\t", closestSurfacePoint.x, closestSurfacePoint.y);
			}
			std::cout << std::endl;
		}
	}
};
