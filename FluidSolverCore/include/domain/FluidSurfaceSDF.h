#pragma once

#include <queue>
#include <cstdint>

#include "math/dataStructures/ScalarField.h"
#include "domain/MarkerParticle.h"


class FluidSurfaceSDF_2D : public ScalarField2D {
public:
	FluidSurfaceSDF_2D(int width, int height, float cellWidth) :
		ScalarField2D(width, height, cellWidth),
		m_levelSet(width, height, cellWidth),
		m_knownCellsGrid(width, height, cellWidth) { }

	void update(const std::vector<MarkerParticle>& markerParticles, unsigned int depth);


private:
	struct SDFCell {
		Vec2f position;
		Vec2f closestNeighbourPos;
		float estimatedSD;

		friend bool operator >(const SDFCell& lhs, const SDFCell& rhs) {
			return lhs.estimatedSD > rhs.estimatedSD;
		}
	};

	ScalarField2D m_levelSet;
	// unkown => 0, known => 1
	Grid2D<uint8_t> m_knownCellsGrid;
	typedef std::priority_queue<SDFCell, std::vector<SDFCell>, std::greater<SDFCell>> MinHeapPQ;
	MinHeapPQ m_unkownSDQueue;

	void updateLevelSet(ScalarField2D& levelSet, const std::vector<MarkerParticle>& markerParticles);
	void calculateSDF(unsigned int depth);
};
