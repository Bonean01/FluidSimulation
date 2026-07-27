#include "math/dataStructures/ScalarField.h"
#include "math/dataStructures/VectorField.h"
#include "math/dataStructures/MACGrid.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace Catch;

TEST_CASE("Bilinear Interpolation") {
	ScalarField2D scalarField{ 4, 4, 1.0f };
	scalarField.setValue(0, 0, 1.0f);
	scalarField.setValue(1, 0, 2.0f);
	scalarField.setValue(0, 1, 3.0f);
	scalarField.setValue(1, 1, 4.0f);
	float res = scalarField.sampleBilinear(0.5f, 0.5f);
	CHECK_THAT(res, Matchers::WithinRel(2.5f, 0.001f));

	VectorField2D vectorField{ 4, 4, 1.0f };
	vectorField.setValue(0, 0, { 1.0f, 4.0f });
	vectorField.setValue(1, 0, { 2.0f, 3.0f });
	vectorField.setValue(0, 1, { 3.0f, 2.0f });
	vectorField.setValue(1, 1, { 4.0f, 1.0f });
	Vec2f res1 = vectorField.sampleBilinear(0.5f, 0.5f);
	CHECK_THAT(res1.x, Matchers::WithinRel(2.5f, 0.001f));
	CHECK_THAT(res1.y, Matchers::WithinRel(2.5f, 0.001f));

	MACGrid2D macGrid{ 2, 2, 1.0f };
	macGrid.setCellValue(0, 0, { 1.0f, 4.0f });
	Vec2f res2 = macGrid.sampleBilinear(0, 0);
	CHECK_THAT(res2.x, Matchers::WithinRel(1.0f, 0.001f));
	CHECK_THAT(res2.y, Matchers::WithinRel(4.0f, 0.001f));
}
