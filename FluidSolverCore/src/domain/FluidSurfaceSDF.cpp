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

            // obtain the minimum distance and the relative position of the closest surface point
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


    // WE STILL NEED TO ESTIMATE THEIR SIGNED DISTANCE AND CLOSEST SURFACE POINT
    for (int j = 0; j < m_height; j++) {
        for (int i = 0; i < m_width; i++) {
            const SDFCellData& current = m_SDFCellData.getValue(i, j);
            if (!current.known) continue;
    
            auto neighbours = getNeighbours(i, j);
            for (SDFCellData* neighbour : neighbours) {
                if (neighbour == nullptr) continue;
                if (!neighbour->known) {
                    neighbour->estimatedSD = current.estimatedSD;
                    m_unknownsQueue.emplace(neighbour);
                }

            }
        }
    }
    

    while (!m_unknownsQueue.empty()) {
        SDFCellData* current = m_unknownsQueue.top().cellData;
        m_unknownsQueue.pop();
        current->known = true;

        float minDist = std::numeric_limits<float>::infinity();
        Vec2f closestSurfacePoint;
        auto neighbours = getNeighbours(*current);

        for (SDFCellData* neighbour : neighbours) {
            if (neighbour == nullptr) continue;
            if (!neighbour->known) continue;
            
            float distance = (neighbour->closestSurfacePointPos - static_cast<Vec2f>(current->position)).magnitude();
            if (distance < minDist) {
                minDist = distance;
                closestSurfacePoint = neighbour->closestSurfacePointPos;
            }
        }
    }

    
    #pragma omp parallel for
    for (int j = 0; j < m_height; j++) {
        for (int i = 0; i < m_width; i++) {
            const SDFCellData& current = m_SDFCellData.getValue(i, j);
            this->setValue(i, j, current.estimatedSD);
        }
    }
}
