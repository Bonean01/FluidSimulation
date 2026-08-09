#include "steps/Diffusion.h"

#include "math/operators/Staggered.h"


void Diffusion::execute(StaggeredVectorField2D& velocityField, const StaggeredGrid2D<BoundaryData>& boundaryData, float kinematicViscosity, float timeStep, unsigned int iterationCount) {
	using enum VectorComponent;

	float dx = velocityField.cellWidth();
	float alpha = kinematicViscosity * timeStep / dx;
	float beta = 1 + 4 * alpha;

	for (unsigned int k = 0; k < iterationCount; k++) {
		Staggered::laplacian(m_laplacianField, velocityField);
		
		diffuseComponent(X, velocityField, boundaryData, kinematicViscosity, timeStep, iterationCount, alpha, beta);
		diffuseComponent(Y, velocityField, boundaryData, kinematicViscosity, timeStep, iterationCount, alpha, beta);

		if (k < iterationCount - 1)
			std::swap(m_auxStaggeredVectorField, velocityField);
	}
}


void Diffusion::diffuseComponent(const VectorComponent& C, StaggeredVectorField2D& velocityField, const StaggeredGrid2D<BoundaryData>& boundaryData, float kinematicViscosity, float timeStep, unsigned int iterationCount, float alpha, float beta) {
	int width = velocityField.getValuesWidth(C);
	int height = velocityField.getValuesHeight(C);

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			if (BoundaryUtils::hasBoundaryPrescribedVelocity(C, i, j, boundaryData)) continue;

			float currentVel = velocityField.getEdgeValue(C, i, j);
			float laplacian = m_laplacianField.getEdgeValue(C, i, j);
			float newVel = (currentVel + alpha * laplacian) / beta;
			m_auxStaggeredVectorField.setEdgeValue(C, i, j, newVel);
		}
	}
}
