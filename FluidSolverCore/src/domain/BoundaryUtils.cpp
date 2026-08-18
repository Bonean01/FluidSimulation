#include "domain/BoundaryUtils.h"
#include <omp.h>


namespace BoundaryUtils {
    void applyVelocityBoundaryConditions(StaggeredVectorField2D& velocityField, const StaggeredGrid2D<BoundaryData>& boundaryData) {
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


    bool hasPrescribedVelocity(const BoundaryData& boundaryData) {
        return boundaryData.velocityBoundaryCondition == BoundaryCondition::Dirichlet;
    }
}
