#include "Vector3.h"
#include "Matrix4x4.h"
#include <cassert>
#include <cmath>
#pragma once
void Move(Vector3& transform, Vector3& move);
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
Matrix4x4 MakeOrthographicMatrix(
    float left, float top, float right, float bottom, float nearClip, float farClip);
Matrix4x4 MakeViewportMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth);
//class MathFunction
//{
//public:
//	static void Move(Vector3& transform, Vector3& move);
//	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
//	static Matrix4x4 MakeRotateXMatrix(float radian);
//	static Matrix4x4 MakeRotateYMatrix(float radian);
//	static Matrix4x4 MakeRotateZMatrix(float radian);
//	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
//	static Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
//	static Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);
//};