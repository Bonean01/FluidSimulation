#include <cmath>
#include "math/dataStructures/Vector.h"

Vector2 Vector2::lerp(const Vector2& A, const Vector2& B, float t) {
	Vector2 res{};
	res.x = std::lerp(A.x, B.x, t);
	res.y = std::lerp(A.y, B.y, t);
	return res;
}


Vector2 Vector2::biLerp(const Vector2& A, const Vector2& B, const Vector2& C, const Vector2& D, float tX, float tY) {
	Vector2 E = lerp(A, B, tX);
	Vector2 F = lerp(C, D, tX);
	return lerp(E, F, tY);
}
