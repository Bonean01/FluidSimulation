#include <cmath>
#include <limits>

#include "domain/FluidSurface.h"
#include "utils/profiling/ScopeProfiler.h"

#include <iostream>


void FluidSurface::update(const std::vector<MarkerParticle>& markerParticles, unsigned int depth) {
    ScopeProfiler p{ "Updating Fluid Surface" };

    updateLevelSet(m_levelSet, markerParticles);
    resetSurfaceData();
    setClosestCellsSD();
    populateWithClosestNeighbours(m_unknownsQueue);
    calculateSDF(depth);
}


std::array<SurfaceData*, 8> FluidSurface::getNeighbours(int posX, int posY) {
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


std::array<float, 8> FluidSurface::getNeighbourSignedDistances(int posX, int posY) {
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


unsigned int FluidSurface::getNeighbourDepth(SurfaceData* neighbour) {
    unsigned int minDepth = std::numeric_limits<int>::max();
    for (auto* ady : getNeighbours(*neighbour)) {
        if (ady != nullptr && ady->depth < minDepth) minDepth = ady->depth;
    }
    return minDepth + 1;
}


SurfaceData* FluidSurface::extractNext(MinHeapPQ& queue) {
    SurfaceData* next = queue.top().surfaceData;
    queue.pop();
    return next;
}


bool FluidSurface::isInsideFluid(const SurfaceData& current) {
    int posX = current.position.x;
    int posY = current.position.y;

    return m_levelSet.getValue(posX, posY) < 0;
}


void FluidSurface::updateLevelSet(ScalarField2D& levelSet, const std::vector<MarkerParticle>& markerParticles) {
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


void FluidSurface::resetSurfaceData() {
    #pragma omp parallel for
    for (int j = 0; j < m_height; j++) {
        for (int i = 0; i < m_width; i++) {
            SurfaceData& current = this->at(i, j);
            current.known = false;
            current.estimatedSD = std::numeric_limits<float>::infinity();
            current.depth = std::numeric_limits<int>::max();
            current.position = { i, j };
        }
    }
}


void FluidSurface::setClosestCellsSD() {
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

            SurfaceData& current = this->at(i, j);
            float currentLS = m_levelSet.getValue(i, j);

            if (minDistance != std::numeric_limits<float>::infinity()) {
                current.estimatedSD = currentLS < 0 ? -minDistance : minDistance;
                current.closestSurfacePointPos = static_cast<Vec2f>(current.position) + static_cast<Vec2f>(minRelPos) / minRelPos.magnitude() * minDistance;
                current.known = true;
                current.depth = 1;
            }
        }
    }
}


void FluidSurface::populateWithClosestNeighbours(MinHeapPQ& queue) {
    // Append neighbouring cells to a priority queue keyed by known distance
    // we need a mutex for controlling access to the queue
    for (int j = 0; j < m_height; j++) {
        for (int i = 0; i < m_width; i++) {
            const SurfaceData& current = this->getValue(i, j);
            if (!current.known) continue;

            auto neighbours = getNeighbours(current);
            for (SurfaceData* neighbour : neighbours) {
                if (neighbour == nullptr) continue;
                if (!neighbour->known && !neighbour->inQueue) {
                    neighbour->estimatedSD = current.estimatedSD;
                    neighbour->inQueue = true;
                    neighbour->depth = 2;
                    queue.emplace(neighbour);
                }
            }
        }
    }
}


void FluidSurface::calculateSDF(unsigned int depth) {  
    while (!m_unknownsQueue.empty()) {
        SurfaceData* current = extractNext(m_unknownsQueue);
        if (current == nullptr) continue;

        current->known = true;
        current->inQueue = false;

        float minDist = std::numeric_limits<float>::infinity();

        // Iterate over all neighbours (3x3)
        for (SurfaceData* neighbour : getNeighbours(*current)) {
            if (neighbour == nullptr) continue;

            if (neighbour->known) {
                float distance = (neighbour->closestSurfacePointPos - static_cast<Vec2f>(current->position)).magnitude();

                // If current is closer than a neighbour mark the neighbour as unknown again
                if (distance < std::abs(neighbour->estimatedSD)) {
                    neighbour->known = false;
                    neighbour->inQueue = true;
                    m_unknownsQueue.emplace(neighbour);
                }
                // Take the closest point to the surface and its distance
                if (distance < minDist) {
                    minDist = distance;
                    current->closestSurfacePointPos = neighbour->closestSurfacePointPos;
                }
            }
            else {
                // If the max depth hasn't been reached add the unknown neighbours to the queue
                neighbour->depth = getNeighbourDepth(neighbour);
                if (neighbour->depth <= depth) {
                    neighbour->inQueue = true;
                    m_unknownsQueue.emplace(neighbour);
                }
            }
        }

        // Determine if current is inside or outside and set it's signed distance
        float signedDistance = isInsideFluid(*current) ? -minDist : minDist;
        current->estimatedSD = signedDistance;
    }
}
