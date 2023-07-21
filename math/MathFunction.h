#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include <cassert>
#include <cmath>

// ベクトルの加算
Vector3 Add(const Vector3& v1, const Vector3& v2);
// ベクトルの減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
// ベクトルの積
Vector3 Multiply(const Vector3& v1, const Vector3& v2);
Vector3 Multiply(const Vector3& v1, const float speed);
//内積
float Dot(const Vector3& v1, const Vector3& v2);
// 長さ
float Length(const Vector3& v);
// 正規化
Vector3 Normalize(const Vector3& v);
// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);
// 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
// ベクトル変換
Vector3 TransformNormal(const Vector3& vector, const Matrix4x4& matrix);
// X軸周りの回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
// Y軸周りの回転行列
Matrix4x4 MakeRotateYMatrix(float radian);
// Z軸周りの回転行列
Matrix4x4 MakeRotateZMatrix(float radian);
// アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
// 正射影行列
Matrix4x4 MakeOrthographicMatrix(
    float left, float top, float right, float bottom, float nearClip, float farClip);
// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth);
// 線形補間
Vector3 Leap(const Vector3& v1, const Vector3& v2, float t);
// 球面線形補間
Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);