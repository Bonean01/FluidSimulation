#pragma once

#include <concepts>

struct Vector2 {
	float x, y;
	Vector2(float x = 0, float y = 0) : x(x), y(y) {}
	static Vector2 lerp(const Vector2& A, const Vector2& B, float t);
	static Vector2 biLerp(const Vector2& A, const Vector2& B, const Vector2& C, const Vector2& D, float tX, float tY);

	Vector2& operator *=(float n) {
		x *= n;
		y *= n;
		return *this;
	}

	friend Vector2 operator *(float n, const Vector2& vec) {
		return { n * vec.x, n * vec.y };
	}

	Vector2& operator +=(const Vector2& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	friend Vector2 operator +(const Vector2& vec1, const Vector2& vec2) {
		return { vec1.x + vec2.x, vec1.y + vec2.y };
	}
};
