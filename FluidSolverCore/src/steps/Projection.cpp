#include "steps/Projection.h"


void Projection::execute(StaggeredVectorField2D& velocityField, const ScalarField2D& pressureField, const Grid2D<CellData>& cellData, float density, float timeStep) {
	using enum VectorComponent;

	projectComponent<X>(velocityField, pressureField, cellData, density, timeStep);
	projectComponent<Y>(velocityField, pressureField, cellData, density, timeStep);	
}
