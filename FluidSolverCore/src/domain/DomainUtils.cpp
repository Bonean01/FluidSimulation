#include "domain/DomainUtils.h"

#include <omp.h>

#include "utils/profiling/ScopeProfiler.h"


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


    // Creates 4 marker particles per fluid cell
    void populateMarkerParticles(std::vector<MarkerParticle>& markerParticles, const Grid2D<CellData>& cellData) {
        for (int j = 0; j < cellData.height(); j++) {
            for (int i = 0; i < cellData.width(); i++) {
                CellData current = cellData.getValue(i, j);
                if (current.cellType == CellType::Fluid) {
                    float cellWidth = cellData.cellWidth();
                    Vec2f cellCenter = cellWidth * Vec2f(i + 0.5f, j + 0.5f);

                    float quarterCellWidth = cellWidth / 4;
                    markerParticles.emplace_back(MarkerParticle(Vec2f(cellCenter.x - quarterCellWidth, cellCenter.y + quarterCellWidth)));
                    markerParticles.emplace_back(MarkerParticle(Vec2f(cellCenter.x + quarterCellWidth, cellCenter.y + quarterCellWidth)));
                    markerParticles.emplace_back(MarkerParticle(Vec2f(cellCenter.x - quarterCellWidth, cellCenter.y - quarterCellWidth)));
                    markerParticles.emplace_back(MarkerParticle(Vec2f(cellCenter.x - quarterCellWidth, cellCenter.y - quarterCellWidth)));
                }
            }
        }
    }


    void updateSurfaceSDF(ScalarField2D& surfaceSDF, const std::vector<MarkerParticle>& markerParticles, unsigned int depth) {
        // Set cells with makers to -1 and cells without them to +1

        // Smooth the field with weighted averages to avoid stair-step artifacts

        // Calculate where the linear interpolant becomes 0, and set the signed distance of nearby cells

        // Append neighbouring cells to a priority queue keyed by known distance
        // Repeat and update distance of neighbouring cells accordingly
        // Stop at "depth" to allow for narrow band methods
    }


    void extrapolateVelocity(StaggeredVectorField2D& velocityField, const Grid2D<CellData>& cellData) {
        // Force that taking the directional derivative of the extrapolated velocity in the direction of
        // the gradient of the SDF returns 0 (all the points between a point and the closest known 
        // velocity should contain the same extrapolated velocity)
    }


    void updateCellData(Grid2D<CellData>& cellData, const std::vector<MarkerParticle>& markerParticles) {
        
    }
}
