#pragma once

#include "math/dataStructures/ScalarField.h"

#include "domain/MarkerParticle.h"

class FluidSurfaceSDF_2D : ScalarField2D {
public:
	FluidSurfaceSDF_2D(int width, int height, float cellWidth) :
		ScalarField2D(width, height, cellWidth),
		m_levelSet(width, height, cellWidth) { }

	void update(const std::vector<MarkerParticle>& markerParticles, unsigned int depth);

private:
	ScalarField2D m_levelSet;

	void updateLevelSet(ScalarField2D& levelSet, const std::vector<MarkerParticle>& markerParticles);
	void calculateSDF(unsigned int depth);
};
