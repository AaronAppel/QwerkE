#pragma once

//////////////////////////////
// Vector.h
// A simple vector class for storing game entity transform data
// with helper and utility methods.
//////////////////////////////

// TODO: Minor efficiency improvement. in operator+=, should I change to use
// { x += value; } or keep { x = x + value; } ? What is more register efficient?

// TODO: Look at operator== for float value comparison
// TODO: inline?
// TODO: fequal

#include <cmath> // sqrt

#include "Libraries/FlatheadGames/MathHelpers.h"

struct Vector2
{
	float x = 0.f;
	float y = 0.f;

	Vector2() = default;
	Vector2(const float value) { x = value; y = value; }
	Vector2(const float valueX, const float valueY) { x = valueX; y = valueY; }
	Vector2(const int value) { x = (float)value; y = (float)value; }
	Vector2(const int valueX, const int valueY) { x = (float)valueX; y = (float)valueY; }

	~Vector2() {}

	Vector2 operator +(const float value) const { return Vector2(x + value, y + value); }
	Vector2 operator +(const Vector2& value) const { return Vector2(x + value.x, y + value.y); }

	Vector2 operator -(const float value) const { return Vector2(x - value, y - value); }
	Vector2 operator -(const Vector2& value) const { return Vector2(x - value.x, y - value.y); }

	Vector2 operator *(const float value) const { return Vector2(x * value, y * value); }
	Vector2 operator *(const Vector2& value) const { return Vector2(x * value.x, y * value.y); }

	Vector2 operator /(const float value) const { return Vector2(x / value, y / value); }
	Vector2 operator /(const Vector2& value) const { return Vector2(x / value.x, y / value.y); }

	void operator +=(const float value) { x = x + value; y = y + value; }
	void operator +=(const Vector2& value) { x = x + value.x; y = y + value.y; }

	void operator -=(const float value) { x = x - value; y = y - value; }
	void operator -=(const Vector2& value) { x = x - value.x; y = y - value.y; }

	void operator *=(const float value) { x = x * value; y = y * value; }
	void operator *=(const Vector2& value) { x = x * value.x; y = y * value.y; }

	void operator /=(const float value) { x = x / value; y = y / value; }
	void operator /=(const Vector2& value) { x = x / value.x; y = y / value.y; }

	bool operator ==(const float value) { return x == value && y == value; }
	bool operator ==(const Vector2& value) { return x == value.x && y == value.y; }

	bool operator !=(const float value) { return x != value || y != value; }
	bool operator !=(const Vector2& value) { return x != value.x || y != value.y; }

	// TODO: Rewrite
	float LengthSquared() const { return x * x + y * y; }
	float Length() const { return sqrtf(x*x + y * y); }
	Vector2 GetNormalized() const { float len = Length(); if (fequal(len, 0.f)) return Vector2(x, y); len = 1.f / len; return Vector2(x*len, y*len); }
	Vector2 Normalize() { float len = Length(); if (!fequal(len, 0)) { x /= len; y /= len; } return *this; }
	// TODO: write... add, set, scale, cross, dot methods
	float Distance(const Vector2& otherVector) { Vector2 difference = *this - otherVector; return sqrt(difference.x * difference.x + difference.y * difference.y); }
};

struct Vector3
{
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	Vector3() = default;
	Vector3(const float value) { x = value; y = value; z = value; }
	Vector3(const float valueX, float valueY, float valueZ) { x = valueX; y = valueY; z = valueZ; }
	Vector3(const int value) { x = (float)value; y = (float)value; z = (float)value; }
	Vector3(const int valueX, int valueY, int valueZ) { x = (float)valueX; y = (float)valueY; z = (float)valueZ; }

	static Vector3 Zero() { return Vector3(0.f, 0.f, 0.f); }

	Vector3 operator +(const float value) const { return Vector3(x - value, y - value, z - value); }
	Vector3 operator +(const Vector3& value) const { return Vector3(x + value.x, y + value.y, z + value.z); }

	Vector3 operator -(const float value) const { return Vector3(x - value, y - value, z - value); }
	Vector3 operator -(const Vector3& value) const { return Vector3(x - value.x, y - value.y, z - value.z); }

	Vector3 operator *(const float value) const  { return Vector3(x * value, y * value, z * value); }
	Vector3 operator *(const Vector3& value) const { return Vector3(x * value.x, y * value.y, z * value.z); }

	Vector3 operator /(const float value) const  { return Vector3(x / value, y / value, z / value); }
	Vector3 operator /(const Vector3& value) const  { return Vector3(x / value.x, y / value.y, z / value.z); }

	void operator +=(const float value) { x = x + value; y = y + value; z = z + value; }
	void operator +=(const Vector3& value) { x = x + value.x; y = y + value.y; z = z + value.z; }

	void operator -=(const float value) { x = x - value; y = y - value; z = z - value; }
	void operator -=(const Vector3& value) { x = x - value.x; y = y - value.y; z = z - value.z; }

	void operator *=(const float value) { x = x * value; y = y * value; z = z * value; }
	void operator *=(const Vector3& value) { x = x * value.x; y = y * value.y; z = z * value.z;}

	void operator /=(const float value) { x = x / value; y = y / value; z = z / value; }
	void operator /=(const Vector3& value) { x = x / value.x; y = y / value.y; z = z / value.z; }

	bool operator ==(const float value) { return x == value && y == value && z == value; }
	bool operator ==(const Vector3& value) { return x == value.x && y == value.y && z == value.z; }

	bool operator !=(const float value) { return x != value || y != value || z != value; }
	bool operator !=(const Vector3& value) { return x != value.x || y != value.y || z != value.z; }

	// TODO: Rewrite
	float LengthSquared() const { return x*x + y*y + z*z; }
	float Length() const { return sqrtf(x*x + y * y + z * z); }
	Vector3 GetNormalized() const { float len = Length(); if( fequal(len,0.f) ) return Vector3(x,y,z); len = 1.f/len; return Vector3(x*len, y*len, z*len);}
	Vector3 Normalize() { float len = Length(); if (!fequal(len, 0)) { x /= len; y /= len; z /= len; } return *this; }
	Vector3 Cross(const Vector3& o) const { return Vector3((y*o.z - z * o.y), (z*o.x - x * o.z), (x*o.y - y * o.x)); }
	float Dot(const Vector3 &o) const { return x * o.x + y * o.y + z * o.z; }
	float Distance(Vector3 otherVector) { Vector3 difference = *this - otherVector; return sqrt(difference.x * difference.x + difference.y * difference.y + difference.z * difference.z); }

	float DistanceXZ(Vector3 a_TipPosition, Vector3 a_TailPosition)
	{
		Vector3 difference = a_TipPosition - a_TailPosition;
		return sqrt(difference.x * difference.x + difference.z * difference.z);
	}

	float DistanceXY(Vector3 a_TipPosition, Vector3 a_TailPosition)
	{
		Vector3 difference = a_TipPosition - a_TailPosition;
		return sqrt(difference.x * difference.x + difference.y * difference.y);
	}

	float AngleXZRad(Vector3 a_TipPosition, Vector3 a_TailPosition)
	{
		Vector3 difference = a_TipPosition - a_TailPosition;
		float angleRadians = atan2(difference.z, difference.x);
		return QwerkE::Math::ClampRollover(0.f, angleRadians, 6.283185f);
	}
};

struct Vector4
{
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	float w = 0.f;

	Vector4() = default;
	Vector4(const float value) { x = value; y = value; }
	Vector4(const float valueX, const float valueY, float valueZ, float valueW) { x = valueX; y = valueY; z = valueZ; w = valueW; }
	Vector4(const int value) { x = (float)value; y = (float)value; z = (float)value; w = (float)value; }
	Vector4(const int valueX, const int valueY, const int valueZ, const int valueW) { x = (float)valueX; y = (float)valueY; z = (float)valueZ; w = (float)valueW; }
};

typedef Vector2 vec2;
typedef Vector3 vec3;
typedef Vector4 vec4;
