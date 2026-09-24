#include "domain/DomainUtils.h"

#include <omp.h>
#include <cmath>

#include "utils/profiling/ScopeProfiler.h"
#include "math/operators/Staggered.h"
#include "domain/CellConfig.h"


void Domain::applyVelocityBoundaryConditions(StaggeredVectorField2D& velocityField) const {
    ScopeProfiler p{ "Velocity BCs" };
    using enum VectorComponent;

    applyVelocityBCsToComponent(X, velocityField, m_boundaryData);
    applyVelocityBCsToComponent(Y, velocityField, m_boundaryData);
}


void Domain::applyVelocityBCsToComponent(const VectorComponent& C, StaggeredVectorField2D& velocityField, const StaggeredGrid2D<BoundaryData>& boundaryData) const {
    using enum VectorComponent;

    int width = velocityField.getValuesWidth(C);
    int height = velocityField.getValuesHeight(C);
    
    #pragma omp parallel for
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            const BoundaryData& currentData = boundaryData.getEdgeValue(C, i, j);
            applyVelocityBCsToEdge(C, i, j, velocityField, currentData);
        }
    }
}


void Domain::applyVelocityBCsToEdge(const VectorComponent& C, int i, int j, StaggeredVectorField2D& velocityField, const BoundaryData& boundaryData) const {
    using enum VectorComponent;

    switch (boundaryData.velocityBoundaryCondition) {
        using enum BoundaryCondition;

        case Dirichlet:
            velocityField.setEdgeValue(C, i, j, boundaryData.prescribedVelocity.get(C));
            break;

        case HomogeneousNeumann:
            // Since StaggeredGrid2D clamps values outside the domain, a homogeneous neumann boundary 
            // condition (∂u / ∂n = 0) gets applied if there's nothing overwriting it (only at the edges of the domain)
            break;
    }
}


bool Domain::hasEdgePrescribedVelocity(const VectorComponent& C, int i, int j) const {
    BoundaryData boundaryData = m_boundaryData.getEdgeValue(C, i, j);
    return boundaryData.velocityBoundaryCondition == BoundaryCondition::Dirichlet;
}


bool Domain::isFluidEdge(const VectorComponent& C, int i, int j) const {
    bool isFluid0 = (C == VectorComponent::X)
        ? m_cellData.getValue(i - 1, j).cellType == CellType::Fluid
        : m_cellData.getValue(i, j - 1).cellType == CellType::Fluid;
    bool isFluid1 = m_cellData.getValue(i, j).cellType == CellType::Fluid;
    return isFluid0 || isFluid1;
}


void Domain::setCell(int i, int j, const CellData& cellData, const BoundaryData& boundaryData) {
    using enum VectorComponent;

    m_cellData.setValue(i, j, cellData);
	m_boundaryData.setEdgeValue(X, i, j, boundaryData);
	m_boundaryData.setEdgeValue(X, i + 1, j, boundaryData);
	m_boundaryData.setEdgeValue(Y, i, j, boundaryData);
	m_boundaryData.setEdgeValue(Y, i, j + 1, boundaryData);
}


void Domain::extrapolateVelocity(StaggeredVectorField2D& velocityField, unsigned int depth) {
    // Force that taking the directional derivative of the extrapolated velocity in the direction of
    // the gradient of the SDF returns 0 (all the points between a point and the closest known
    // velocity should contain the same extrapolated velocity)
    ScopeProfiler p{"Velocity Extrapolation"};

    if (m_markerParticles.empty()) return;

    int width = m_fluidSurface.width();
    int height = m_fluidSurface.height();
    m_fluidSurface.update(m_markerParticles, depth);

    #pragma omp parallel for
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            SurfaceData current = m_fluidSurface.getValue(i, j);
            if (!current.known || m_cellData.getValue(i, j).cellType == CellType::Fluid) continue;

            Vec2f vel = velocityField.sampleBilinear(current.closestSurfacePointPos);
            m_extrapolatedVelField.setValue(i, j, vel);
        }
    }


    std::array<VectorComponent, 2> components = { VectorComponent::X, VectorComponent::Y };
    for (VectorComponent C : components) {
        int width = velocityField.getValuesWidth(C);
        int height = velocityField.getValuesHeight(C);
        #pragma omp parallel for
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                SurfaceData cell0 = (C == VectorComponent::X)
                    ? m_fluidSurface.getValue(i - 1, j)
                    : m_fluidSurface.getValue(i, j - 1);
                SurfaceData cell1 = m_fluidSurface.getValue(i, j);

                if (!cell0.known || !cell1.known || isFluidEdge(C, i, j)) continue;

                float vel0 = (C == VectorComponent::X)
                    ? m_extrapolatedVelField.getValue(i - 1, j).x
                    : m_extrapolatedVelField.getValue(i, j - 1).y;
                float vel1 = (C == VectorComponent::X) 
                    ? m_extrapolatedVelField.getValue(i, j).x 
                    : m_extrapolatedVelField.getValue(i, j).y;

                float average = (vel0 + vel1) / 2;
                velocityField.setEdgeValue(C, i, j, average);
                
            }
        }
    }
}


void Domain::updateCellData() {
    if (m_markerParticles.empty()) return;

    int width = m_cellData.width();
    int height = m_cellData.height();
    
    #pragma omp parallel for
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            CellData& currentCell = m_cellData.at(i, j);
            if (currentCell.cellType == CellType::Solid) continue;
            else currentCell.cellType = CellType::Void;
        }
    }

    #pragma omp parallel for
    for (int i = 0; i < m_markerParticles.size(); i++) {
        const MarkerParticle& particle = m_markerParticles.at(i);
        int cellPosX = static_cast<int>(std::floor(particle.position.x));
        int cellPosY = static_cast<int>(std::floor(particle.position.y));
        CellData& currentCell = m_cellData.at(cellPosX, cellPosY);
        currentCell.cellType = CellType::Fluid;
    }
}


void Domain::flood() {
    int width = m_cellData.width();
    int height = m_cellData.height();
    CellConfig fluid = {{CellType::Fluid}, {BoundaryCondition::None}};

    #pragma omp parallel for
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            CellData& current = m_cellData.at(i, j);
            if (current.cellType == CellType::Void) current.cellType = CellType::Fluid;
        }
    }
}


void Domain::populateMarkerParticles()  {
    int width = m_cellData.width();
    int height = m_cellData.height();

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            CellType cellType = m_cellData.getValue(i, j).cellType;
            if (cellType != CellType::Fluid) continue;
            Vec2f cellCenter = Vec2f(i + 0.5f, j + 0.5f);
            m_markerParticles.emplace_back(MarkerParticle(Vec2f(cellCenter.x + 0.25f, cellCenter.y + 0.25f)));
            m_markerParticles.emplace_back(MarkerParticle(Vec2f(cellCenter.x - 0.25f, cellCenter.y - 0.25f)));
            m_markerParticles.emplace_back(MarkerParticle(Vec2f(cellCenter.x + 0.25f, cellCenter.y - 0.25f)));
            m_markerParticles.emplace_back(MarkerParticle(Vec2f(cellCenter.x - 0.25f, cellCenter.y + 0.25f)));
        }
    }
}
