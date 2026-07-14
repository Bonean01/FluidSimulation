#include <iostream>
#include "math/VectorField.h"
#include "math/Vector.h"

int main(int argc, char* argv[]) {
	VectorField2D field{2, 2, 1.0f};
	field.setValue(0, 0, { 0.0f, 0.0f });
	field.setValue(0, 1, { 0.0f, 1.0f });
	field.setValue(1, 0, { 1.0f, 0.0f });
	field.setValue(1, 1, { 1.0f, 1.0f });
	Vector2 result = field.sampleBilinear(0.5f, 0.5f);
	std::cout << "result: (" << result.x << ", " << result.y << ")" << std::endl;
}
