#include "steps/Advection.h"


void Advection::execute(StaggeredVectorField2D& velocityField, const Grid2D<CellData>& cellData, float timeStep) {
	using enum VectorComponent;

	int width = velocityField.width();
	int height = velocityField.height();
	if (m_auxStaggeredVectorField.width() != width || m_auxStaggeredVectorField.height() != height) return;

	advectComponent<X>(velocityField, cellData, timeStep);
	advectComponent<Y>(velocityField, cellData, timeStep);

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
