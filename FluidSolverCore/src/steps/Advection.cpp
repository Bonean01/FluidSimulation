#include "steps/Advection.h"

#include "domain/BoundaryUtils.h"


void Advection::execute(StaggeredVectorField2D& velocityField, const Grid2D<CellData>& cellData, float timeStep) {
	using enum VectorComponent;

	int width = velocityField.width();
	int height = velocityField.height();
	if (m_auxStaggeredVectorField.width() != width || m_auxStaggeredVectorField.height() != height) return;

	// === horizontal ===
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width + 1; i++) {
			if (BoundaryUtils::hasBoundaryPrescribedVelocity(VectorComponent::X, cellData, i, j)) continue;

			Vec2f position = { (float)i - 0.5f, (float)j };
			Vec2f currentVel = velocityField.sampleBilinear(position);
			Vec2f newValue = velocityField.sampleBilinear(position - currentVel * timeStep);
			m_auxStaggeredVectorField.setEdgeValue<X>(i, j, newValue.x);
		}
	}
	// === vertical ===
	for (int j = 0; j < height + 1; j++) {
		for (int i = 0; i < width; i++) {
			if (BoundaryUtils::hasBoundaryPrescribedVelocity(VectorComponent::Y, cellData, i, j)) continue;
			
			Vec2f position = { (float)i, (float)j - 0.5f };
			Vec2f currentVel = velocityField.sampleBilinear(position);
			Vec2f newValue = velocityField.sampleBilinear(position - currentVel * timeStep);
			m_auxStaggeredVectorField.setEdgeValue<Y>(i, j, newValue.y);
		}
	}
	std::swap(velocityField, m_auxStaggeredVectorField);
}


void Advection::execute(ScalarField2D& field, const StaggeredVectorField2D& velocityField, const Grid2D<CellData>& cellData, float timeStep) {
	int width = field.width();
	int height = field.height();
	if (m_auxScalarField.width() != width || m_auxScalarField.height() != height) return;

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			if (cellData.getValue(i, j).cellType == CellType::Solid) { m_auxScalarField.setValue(i, j, 0.0f); continue; }
			Vec2f position = { (float)i, (float)j };
			Vec2f currentVel = velocityField.sampleBilinear(position);
			float newValue = field.sampleBilinear(position - currentVel * timeStep);
			m_auxScalarField.setValue(i, j, newValue);
		}
	}
	std::swap(field, m_auxScalarField);
}
