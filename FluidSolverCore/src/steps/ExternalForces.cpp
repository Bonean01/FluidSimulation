#include <omp.h>

#include "steps/ExternalForces.h"
#include "math/operators/FiniteDifference.h"
#include "math/operators/Staggered.h"


namespace ExternalForces {
	void applyGravity(StaggeredVectorField2D& velocityField, const Domain& domain, float timeStep, float gravity) {
		int width = velocityField.getValuesWidth(VectorComponent::Y);
		int height = velocityField.getValuesHeight(VectorComponent::Y);

		#pragma omp parallel for
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				if (domain.hasEdgePrescribedVelocity(VectorComponent::Y, i, j)) continue;
				float currentVel = velocityField.getEdgeValue(VectorComponent::Y, i, j);
				velocityField.setEdgeValue(VectorComponent::Y, i, j, currentVel + gravity * timeStep);
			}
		}
	}


	void applyContinuumSurfaceForce(StaggeredVectorField2D& velocityField, const Domain& domain, const FluidInterface2D& fluidInterface, const ScalarField2D& volumeFractionField, float density, float timeStep, float surfaceTensionCoefficient) {
		using enum VectorComponent;

		if (surfaceTensionCoefficient == 0.0f) return;

		ScalarField2D curvatureField{ velocityField.width(), velocityField.height(), velocityField.cellWidth() };
		FiniteDifference::curvature(curvatureField, fluidInterface);

		for (VectorComponent C : { X, Y }) {
			int width = velocityField.getValuesWidth(C);
			int height = velocityField.getValuesHeight(C);

			#pragma omp parallel for
			for (int j = 0; j < height; j++) {
				for (int i = 0; i < width; i++) {
					if (domain.hasEdgePrescribedVelocity(C, i, j)) continue;
					Vec2f normal = fluidInterface.getValue(i, j).normal;
					if (normal.x == 0 && normal.y == 0) continue;

					float currentVel = velocityField.getEdgeValue(C, i, j);
					float curvature0 = curvatureField.getValue(i, j);
					float curvature1 = curvatureField.getValue(i - (C == X ? 1 : 0), j - (C == Y ? 1 : 0));
					float curvature = (curvature0 + curvature1) / 2;
					float volumeFractionGradient = Staggered::gradient(C, i, j, volumeFractionField);
					float CSF = curvature * surfaceTensionCoefficient * std::abs(volumeFractionGradient);

					velocityField.setEdgeValue(C, i, j, currentVel + CSF / density * timeStep);
				}
			}
		}
	}
}
