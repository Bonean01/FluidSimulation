#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "math/dataStructures/ScalarField.h"
#include "math/dataStructures/VectorField.h"
#include "math/dataStructures/MACGrid.h"


using namespace Catch;

TEST_CASE("Bilinear Sampling") {
	ScalarField2D scalarField{ 4, 4, 1.0f };
	scalarField.setValue(0, 0, 1.0f);
	scalarField.setValue(1, 0, 2.0f);
	scalarField.setValue(0, 1, 3.0f);
	scalarField.setValue(1, 1, 4.0f);
	float resF = scalarField.sampleBilinear(0.5f, 0.5f);
	CHECK_THAT(resF, Matchers::WithinRel(2.5f, 0.001f));
	resF = scalarField.sampleBilinear(4.0f, 4.0f);
	CHECK_THAT(resF, Matchers::WithinRel(0.0f, 0.001f));

	VectorField2D vectorField{ 4, 4, 1.0f };
	vectorField.setValue(0, 0, { 1.0f, 4.0f });
	vectorField.setValue(1, 0, { 2.0f, 3.0f });
	vectorField.setValue(0, 1, { 3.0f, 2.0f });
	vectorField.setValue(1, 1, { 4.0f, 1.0f });
	Vec2f resV = vectorField.sampleBilinear(0.5f, 0.5f);
	CHECK_THAT(resV.x, Matchers::WithinRel(2.5f, 0.001f));
	CHECK_THAT(resV.y, Matchers::WithinRel(2.5f, 0.001f));

	MACGrid2D macGrid{ 2, 2, 1.0f };
	macGrid.setCellValue(0, 0, { 1.0f, 4.0f });
	resV = macGrid.sampleBilinear(0, 0);
	CHECK_THAT(resV.x, Matchers::WithinRel(1.0f, 0.001f));
	CHECK_THAT(resV.y, Matchers::WithinRel(4.0f, 0.001f));
}
