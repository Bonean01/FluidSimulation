#include "steps/Diffusion.h"

#include "math/operators/Staggered.h"


void Diffusion::execute(StaggeredVectorField2D& velocityField, const Grid2D<CellData>& cellData, float kinematicViscosity, float timeStep, unsigned int iterationCount) {
	using enum VectorComponent;

	float dx = velocityField.cellWidth();
	float alpha = (kinematicViscosity * timeStep) / dx;
	float beta = 1 + 4 * alpha;

	for (unsigned int k = 0; k < iterationCount; k++) {
		Staggered::laplacian(m_laplacianField, velocityField);
		
		diffuseComponent<X>(velocityField, cellData, kinematicViscosity, timeStep, iterationCount, alpha, beta);
		diffuseComponent<Y>(velocityField, cellData, kinematicViscosity, timeStep, iterationCount, alpha, beta);

		if (k < iterationCount)
			std::swap(m_auxStaggeredVectorField, velocityField);
	}
}
