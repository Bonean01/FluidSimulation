#include <cmath>
#include <limits>

#include "domain/FluidSurfaceSDF.h"



void FluidSurfaceSDF_2D::update(const std::vector<MarkerParticle>& markerParticles, unsigned int depth) {
    updateLevelSet(m_levelSet, markerParticles);
    calculateSDF(depth);
}


void FluidSurfaceSDF_2D::updateLevelSet(ScalarField2D& levelSet, const std::vector<MarkerParticle>& markerParticles) {
    // Set cells with makers to -1 and cells without them to +1
    #pragma omp parallel for
    for (int j = 0; j < m_height; j++) {
        for (int i = 0; i < m_width; i++) {
            levelSet.setValue(i, j, 1.0f);
        }
    }

    #pragma omp parallel for
    for (int i = 0; i < markerParticles.size(); i++) {
        const MarkerParticle& particle = markerParticles.at(i);
        int cellPosX = static_cast<int>(std::floor(particle.position.x));
        int cellPosY = static_cast<int>(std::floor(particle.position.y));
        levelSet.setValue(cellPosX, cellPosY, -1.0f);
    }

    // TODO: Smooth the level set with weighted averages to avoid stair-step artifacts
}


void FluidSurfaceSDF_2D::calculateSDF(unsigned int detph) {
    // Calculate where the linear interpolant becomes 0 (between cells where the sign changes),
    // and set the signed distance of nearby cells
    

    // Append neighbouring cells to a priority queue keyed by known distance
    // Repeat and update distance of neighbouring cells accordingly
    // Stop at "depth" to allow for narrow band methods
}
