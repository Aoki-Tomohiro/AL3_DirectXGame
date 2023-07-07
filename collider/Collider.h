#pragma once
#include <stdint.h>
#include "math/MathFunction.h"
/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider {
public:
	//衝突時に呼ばれる関数
	virtual void OnCollision(){};
	virtual Vector3 GetWorldPosition() = 0;
	//半径を取得
	float GetRadius() { return radius_; };
	//半径を設定
	void SetRadius(float radius) { this->radius_ = radius; };

private:
	//衝突半径
	float radius_ = 1.0f;
};