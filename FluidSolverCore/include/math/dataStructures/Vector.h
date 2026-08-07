#pragma once

#include <stdexcept>


enum VectorComponent {
	X,
	Y
};


struct Vec2f {
	float x, y;

	Vec2f(float x = 0, float y = 0) : x(x), y(y) {}
	float get(const VectorComponent& component) const {
		using enum VectorComponent;
		switch (component) {
			case X: return x;
			case Y: return y;
			default: throw std::runtime_error("Vec2f only has X and Y components.");
		}
	}

	static Vec2f lerp(const Vec2f& A, const Vec2f& B, float t);
	static Vec2f biLerp(const Vec2f& A, const Vec2f& B, const Vec2f& C, const Vec2f& D, float tX, float tY);

	Vec2f& operator *=(float n) {
		x *= n;
		y *= n;
		return *this;
	}

	friend Vec2f operator *(float n, const Vec2f& vec) {
		return { n * vec.x, n * vec.y };
	}
	friend Vec2f operator *(const Vec2f& vec, float n) {
		return n * vec;
	}

	friend Vec2f operator /(const Vec2f& vec, float n) {
		return { vec.x / n, vec.y / n };
	}

	Vec2f& operator +=(const Vec2f& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	friend Vec2f operator +(const Vec2f& vec1, const Vec2f& vec2) {
		return { vec1.x + vec2.x, vec1.y + vec2.y };
	}

	friend Vec2f operator -(const Vec2f& vec1, const Vec2f& vec2) {
		return { vec1.x - vec2.x, vec1.y - vec2.y };
	}
	
	friend Vec2f operator -(const Vec2f& vec) {
		return { -vec.x, -vec.y };
	}
};
