#include "domain/DomainUtils.h"

#include <omp.h>
#include <cmath>

#include "utils/profiling/ScopeProfiler.h"
#include "math/operators/Staggered.h"


namespace DomainUtils {
    void applyVelocityBoundaryConditions(StaggeredVectorField2D& velocityField, const StaggeredGrid2D<BoundaryData>& boundaryData) {
        ScopeProfiler p{ "Velocity BCs" };
        using enum VectorComponent;

        applyVelocityBCsToComponent(X, velocityField, boundaryData);
        applyVelocityBCsToComponent(Y, velocityField, boundaryData);
    }


    void applyVelocityBCsToComponent(const VectorComponent& C, StaggeredVectorField2D& velocityField, const StaggeredGrid2D<BoundaryData>& boundaryData) {
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


	void applyVelocityBCsToEdge(const VectorComponent& C, int i, int j, StaggeredVectorField2D& velocityField, const BoundaryData& boundaryData) {
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


    bool hasBoundaryPrescribedVelocity(const BoundaryData& boundaryData) {
        return boundaryData.velocityBoundaryCondition == BoundaryCondition::Dirichlet;
    }


    void extrapolateVelocity(StaggeredVectorField2D& velocityField, const FluidSurfaceSDF_2D& surfaceSDF, const Grid2D<CellData>& cellData) {
        // Force that taking the directional derivative of the extrapolated velocity in the direction of
        // the gradient of the SDF returns 0 (all the points between a point and the closest known 
        // velocity should contain the same extrapolated velocity)

        //StaggeredVectorField2D surfaceSDFGradient = Staggered::gradient(surfaceSDF);
    }


    void updateCellData(Grid2D<CellData>& cellData, const std::vector<MarkerParticle>& markerParticles) {
        int width = cellData.width();
        int height = cellData.height();
        
        #pragma omp parallel for
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                CellData& currentCell = cellData.at(i, j);
                if (currentCell.cellType == CellType::Solid) continue;
                else currentCell.cellType = CellType::Void;
            }
        }

        #pragma omp parallel for
        for (int i = 0; i < markerParticles.size(); i++) {
            const MarkerParticle& particle = markerParticles.at(i);
            int cellPosX = static_cast<int>(std::floor(particle.position.x));
            int cellPosY = static_cast<int>(std::floor(particle.position.y));
            CellData& currentCell = cellData.at(cellPosX, cellPosY);
            currentCell.cellType = CellType::Fluid;
        }
    }
}
