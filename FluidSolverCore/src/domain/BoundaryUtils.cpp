#include "domain/BoundaryUtils.h"


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
        
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                if (not boundaryData.getEdgeValue(C, i, j).hasPrescribedVelocity) continue;
                const BoundaryData& currentData = boundaryData.getEdgeValue(C, i, j);
                applyVelocityBCsToEdge(C, i, j, velocityField, currentData);
            }
        }
    }


	void applyVelocityBCsToEdge(const VectorComponent& C, int i, int j, StaggeredVectorField2D& velocityField, const BoundaryData& boundaryData) {
		using enum VectorComponent;

		switch (boundaryData.boundaryType) {
			using enum BoundaryType;

			case VelocityInlet:
			case NoSlip:
					velocityField.setEdgeValue(C, i, j, boundaryData.prescribedVelocity.get(C));
				break;

			case VelocityOutlet:
				// Since StaggeredGrid2D clamps values outside the domain, a continuity boundary (∂u / ∂n = 0) 
				// gets applied if there's nothing overwriting it (only at the edges)
				break;
		}
	}
}
