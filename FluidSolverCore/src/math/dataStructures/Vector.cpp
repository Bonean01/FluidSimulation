#include <cmath>
#include "math/dataStructures/Vector.h"

Vec2f Vec2f::lerp(const Vec2f& A, const Vec2f& B, float t) {
	Vec2f res{};
	res.x = std::lerp(A.x, B.x, t);
	res.y = std::lerp(A.y, B.y, t);
	return res;
}


Vec2f Vec2f::biLerp(const Vec2f& A, const Vec2f& B, const Vec2f& C, const Vec2f& D, float tX, float tY) {
	Vec2f E = lerp(A, B, tX);
	Vec2f F = lerp(C, D, tX);
	return lerp(E, F, tY);
}
