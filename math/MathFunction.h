#include "Vector3.h"
#include "Matrix4x4.h"
#include <cassert>
#include <cmath>
#pragma once
void Move(Vector3& transform, Vector3& move);
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
float Length(const Vector3& v);
float Dot(const Vector3& v1, const Vector3& v2);
Vector3 Normalize(const Vector3& v);
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
Matrix4x4 MakeOrthographicMatrix(
    float left, float top, float right, float bottom, float nearClip, float farClip);
Matrix4x4 MakeViewportMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth);
Vector3 TransformNormal(const Vector3& vector, const Matrix4x4& matrix);
Vector3 Leap(const Vector3& v1, const Vector3& v2, float t);
Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);