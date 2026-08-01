#include "steps/Diffusion.h"
#include "math/operators/Staggered.h"


void Diffusion::execute(MACGrid2D& velocityField, float kinematicViscosity, float timeStep, int iterationCount) {
	int width = velocityField.width();
	int height = velocityField.height();
	float dx = velocityField.cellWidth();
	float alpha = (kinematicViscosity * timeStep) / dx;
	float beta = 1 + 4 * alpha;

	for (int k = 0; k < iterationCount; k++) {
		Staggered::laplacian(m_laplacianField, velocityField);
		// === horizontal ===
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width + 1; i++) {
				float currentVelX = velocityField.getEdgeX(i, j);
				float laplacianX = m_laplacianField.getEdgeX(i, j);
				float newVelX = (currentVelX + alpha * laplacianX) / beta;
				m_auxMacGrid.setEdgeX(i, j, newVelX);
			}
		}
		// === vertical ===
		for (int j = 0; j < height + 1; j++) {
			for (int i = 0; i < width; i++) {
				float currentVelY = velocityField.getEdgeY(i, j);
				float laplacianY = m_laplacianField.getEdgeY(i, j);
				float newVelY = (currentVelY + alpha * laplacianY) / beta;
				m_auxMacGrid.setEdgeY(i, j, newVelY);
			}
		}
		if (k < iterationCount)
			std::swap(m_auxMacGrid, velocityField);
	}
}
