#include "steps/Diffusion.h"

#include "math/operators/Staggered.h"
#include "domain/BoundaryUtils.h"


void Diffusion::execute(StaggeredVectorField2D& velocityField, const Grid2D<CellData>& cellData, float kinematicViscosity, float timeStep, unsigned int iterationCount) {
	int width = velocityField.width();
	int height = velocityField.height();
	float dx = velocityField.cellWidth();
	float alpha = (kinematicViscosity * timeStep) / dx;
	float beta = 1 + 4 * alpha;

	for (unsigned int k = 0; k < iterationCount; k++) {
		Staggered::laplacian(m_laplacianField, velocityField);
		// === horizontal ===
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width + 1; i++) {
				if (BoundaryUtils::hasBoundaryPrescribedVelocity(VectorComponent::X, cellData, i, j)) continue;

				float currentVelX = velocityField.getEdgeValue<X>(i, j);
				float laplacianX = m_laplacianField.getEdgeValue<X>(i, j);
				float newVelX = (currentVelX + alpha * laplacianX) / beta;
				m_auxStaggeredVectorField.setEdgeValue<X>(i, j, newVelX);
			}
		}
		// === vertical ===
		for (int j = 0; j < height + 1; j++) {
			for (int i = 0; i < width; i++) {
				if (BoundaryUtils::hasBoundaryPrescribedVelocity(VectorComponent::Y, cellData, i, j)) continue;
				
				float currentVelY = velocityField.getEdgeValue<Y>(i, j);
				float laplacianY = m_laplacianField.getEdgeValue<Y>(i, j);
				float newVelY = (currentVelY + alpha * laplacianY) / beta;
				m_auxStaggeredVectorField.setEdgeValue<Y>(i, j, newVelY);
			}
		}
		if (k < iterationCount)
			std::swap(m_auxStaggeredVectorField, velocityField);
	}
}
