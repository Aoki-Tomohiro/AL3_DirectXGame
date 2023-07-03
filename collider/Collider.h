#pragma once
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
	//衝突属性(自分)を取得
	uint32_t GetCollisionAttribute() { return collisionAttribute_; };
	//衝突属性(自分)を設定
	void SetCollisionAttribute(uint32_t collisionAttribute) {
		this->collisionAttribute_ = collisionAttribute;
	};
	//衝突属性(相手)を取得
	uint32_t GetCollisionMask() { return CollisionMask_; };
	//衝突属性(相手)を設定
	void SetCollisionMask(uint32_t CollisionMask) { this->CollisionMask_ = CollisionMask; };

	private:
	//衝突半径
	float radius_ = 1.0f;
	//衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;
	//衝突マスク(相手)
	uint32_t CollisionMask_ = 0xffffffff;
};