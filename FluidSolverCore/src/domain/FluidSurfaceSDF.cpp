#include <cmath>
#include <limits>

#include "domain/FluidSurfaceSDF.h"


#include <iostream>


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


void FluidSurfaceSDF_2D::calculateSDF(unsigned int depth) {
    // Reset
    #pragma omp parallel for
    for (int j = 0; j < m_height; j++) {
        for (int i = 0; i < m_width; i++) {
            SDFCellData& current = m_SDFCellData.at(i, j);
            current.known = false;
            current.estimatedSD = std::numeric_limits<float>::infinity();
            current.depth = 0;
            current.position = { i, j };
        }
    }

    // Calculate where the linear interpolant becomes 0 (between neighbouring cells where the sign changes),
    // calculate the distances and set the current cell to the minimum of them
    #pragma omp parallel for
    for (int j = 0; j < m_height; j++) {
        for (int i = 0; i < m_width; i++) {
            auto neighbourSDs = getNeighbourSignedDistances(i, j);

            float minDistance = std::abs(neighbourSDs[0]);
            Vec2i minRelPos = neighbourRelativePositions[0];

            for (int k = 1; k < neighbourSDs.size(); k++) {
                float distance = std::abs(neighbourSDs[k]);
                if (distance < minDistance) {
                    minDistance = distance;
                    minRelPos = neighbourRelativePositions[k];
                }
            }

            SDFCellData& current = m_SDFCellData.at(i, j);
            float currentLS = m_levelSet.getValue(i, j);

            if (minDistance != std::numeric_limits<float>::infinity()) {
                current.estimatedSD = currentLS < 0 ? -minDistance : minDistance;
                current.closestSurfacePointPos = static_cast<Vec2f>(minRelPos) / minRelPos.magnitude() * minDistance;
                current.known = true;
            }
        }
    }


     //Append neighbouring cells to a priority queue keyed by known distance
     //Repeat and update distance of neighbouring cells accordingly
     //Stop at "depth" to allow for narrow band methods
     //we need a mutex for controlling access to the queue
    for (int j = 0; j < m_height; j++) {
        for (int i = 0; i < m_width; i++) {
            const SDFCellData& current = m_SDFCellData.getValue(i, j);
            if (!current.known) continue;
    
            auto neighbours = getNeighbours(i, j);
            for (auto& neighbour : neighbours) {
                if (neighbour == nullptr) continue;
                if (!neighbour->known) {
                    m_unknownsQueue.emplace(neighbour->position, neighbour->estimatedSD);
                }
            }
        }
    }

    
    while (!m_unknownsQueue.empty()) {
        Vec2i position = m_unknownsQueue.top().position;
        m_unknownsQueue.pop();
        SDFCellData& current = m_SDFCellData.at(position.x, position.y);
        
        if (current.depth > depth) continue;

        float minSurfacePtDst = std::numeric_limits<float>::infinity();
    
        for (auto& neighbour : getNeighbours(position.x, position.y)) {
            if (neighbour == nullptr) continue;
    
            // Find all neighbour cells that have a known signed distance and closest surface point
            if (neighbour->known) {
                Vec2f& closestSurfacePoint = neighbour->closestSurfacePointPos;
                float surfacePtDst = (closestSurfacePoint - static_cast<Vec2f>(position)).magnitude();
    
                // If current is closer than a neighbour mark the neighbour as unknown again
                if (surfacePtDst < std::abs(neighbour->estimatedSD)) neighbour->known = false;
    
                // Take the minimum of the distances
                if (surfacePtDst < minSurfacePtDst) {
                    minSurfacePtDst = surfacePtDst;
                    current.closestSurfacePointPos = closestSurfacePoint;
                }
            }
            if (!neighbour->known) {
                neighbour->depth = current.depth + 1;
                m_unknownsQueue.emplace(neighbour->position, neighbour->estimatedSD);
            }
        }
        // Determine if current is inside or outside the surface and set its signed distance accordingly
        float currentLS = m_levelSet.getValue(position.x, position.y);
        float signedDistance = currentLS < 0 ? -minSurfacePtDst : minSurfacePtDst;
        current.estimatedSD = signedDistance;
        current.known = true;
    }

    
    #pragma omp parallel for
    for (int j = 0; j < m_height; j++) {
        for (int i = 0; i < m_width; i++) {
            const SDFCellData& current = m_SDFCellData.getValue(i, j);
            this->setValue(i, j, current.estimatedSD);
        }
    }
}
