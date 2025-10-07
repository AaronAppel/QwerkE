#pragma once

#include "../MyMath/MyMath.h"

#include "Vectors.h"

inline Vector3f ClosestPointAlongLine(const Vector3f& lineStart, const Vector3f& lineEnd, const Vector3f& point)
{
    Vector3f startToEnd = lineEnd - lineStart;
    float lineLength = startToEnd.Magnitude();

    if (lineLength <= FLT_EPSILON)
    {
        return lineStart;
    }

    Vector3f startToPoint = point - lineStart;
    Vector3f startToEndDirection = startToEnd.Normalized();

    float startToPointDistance = Vector3f::Dot(startToPoint, startToEndDirection);
    startToPointDistance = Math::Clamp(startToPointDistance, 0.f, lineLength); // #NOTE Non-infinite line

    return lineStart + startToEndDirection * startToPointDistance;
}

// #TODO Variations:
// - Infinite line
// - Start, direction, point

inline Vector2f ClosestPointAlongLine(const Vector2f& lineStart, const Vector2f& lineEnd, const Vector2f& point)
{
    Vector2f startToEnd = lineEnd - lineStart;
    float lineLength = startToEnd.Magnitude();

    if (lineLength <= FLT_EPSILON)
    {
        return lineStart;
    }

    Vector2f startToPoint = point - lineStart;
    Vector2f startToEndDirection = startToEnd.Normalized();

    float startToPointDistance = Vector2f::Dot(startToPoint, startToEndDirection);
    startToPointDistance = Math::Clamp(startToPointDistance, 0.f, lineLength);

    return lineStart + startToEndDirection * startToPointDistance;
}

Vector3f RotateAroundAxis(const Vector3f& vector, const Vector3f& axis, float degrees)
{
    // Rodrigues' rotation formula
    // v = v * cos(theta) + (k * v) cross sin(theta) + k *(k dot v) * (1-cos(theta))
    const float angleRadians = degrees * Math::DEG_TO_RAD;

    const Vector3f normalizedAxis = axis.Normalized(); // #TODO User required normalization?

    const float cosTheta = std::cos(angleRadians);
    const float sinTheta = std::sin(angleRadians);
    return vector * cosTheta + normalizedAxis.Cross(vector) * sinTheta + normalizedAxis * (normalizedAxis.Dot(vector)) * (1.0f - cosTheta);
}

Vector3f PointAngleRadius(const Vector3f& point, float degrees, float radius)
{
    const float radians = degrees * Math::DEG_TO_RAD;
    const Vector3f direction = vec3(std::sinf(radians), 0.f, std::cosf(radians)); // #NOTE +Z forward 0 degrees clockwise
    return point + direction * radius;
}

bool PointInSphere(const Vector3f& point, const Vector3f& center, float radius)
{
    return (center - point).Magnitude() < radius;
}

#include <iostream>
std::ostream& operator<<(std::ostream& os, const Vector2f& vec) {
	os << "(" << vec.x << ", " << vec.y << ")";
	return os;
}

std::ostream& operator<<(std::ostream& os, const Vector3f& vec) {
	os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return os;
}

std::ostream& operator<<(std::ostream& os, const Vector4f& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return os;
}

#include <sstream>
#include <string>

// #TODO Write custom string helpers
std::string ToString(const Vector3f& vec) { std::ostringstream oStream; oStream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")"; return oStream.str(); }
void FromString(Vector3f& vec, const std::string& other)
{
    std::istringstream iss(other);
    float tempX, tempY, tempZ;
    char commaOrParens;
    if (!(iss >> commaOrParens) || commaOrParens != '(') return; iss >> std::ws;
    if (!(iss >> tempX)) return; iss >> std::ws;
    if (!(iss >> commaOrParens) || commaOrParens != ',') return; iss >> std::ws;
    if (!(iss >> tempY)) return; iss >> std::ws;
    if (!(iss >> commaOrParens) || commaOrParens != ',') return; iss >> std::ws;
    if (!(iss >> tempZ)) return; iss >> std::ws;
    if (!(iss >> commaOrParens) || commaOrParens != ')') return;
    if (!iss.eof() && iss.peek() != EOF) return;
    vec.Set(tempX, tempY, tempZ);
}