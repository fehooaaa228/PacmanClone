#pragma once

#include <cmath>

struct vec2
{
	float x, y;

	vec2(float x, float y) : x(x), y(y) {}
	vec2() : x(0), y(0) {}

	vec2 operator+(vec2 const& v) { return vec2(x + v.x, y + v.y); }
	vec2 operator-(vec2 const& v) { return vec2(x - v.x, y - v.y); }
	vec2 operator-(const int& a) { return vec2(x - a, y - a); }
	vec2 operator/(vec2 const& v) { return vec2(x / v.x, y / v.y); }
	vec2 operator*(float const& v) { return vec2(x * v, y * v); }
	vec2 operator-() { return vec2(-x, -y); }
	void operator+=(vec2 const& v) { x += v.x; y += v.y; }
	void operator=(vec2 v) { x = v.x; y = v.y; }
	bool operator==(vec2 v) { return x == v.x && y == v.y; }
	bool operator!=(vec2 v) { return !(x == v.x && y == v.y); }
	bool operator>(vec2 v) {
		if (x > v.x && y > v.y) return 1;
		if (x == v.x && y > v.y) return 1;
		if (x > v.x && y == v.y) return 1;
		return 0;
	}
};

static int sign(float v) {
	return (v > 0) ? 1 : (v == 0) ? 0 : -1;
}

static float length(vec2 v) { return sqrt(v.x * v.x + v.y * v.y); }