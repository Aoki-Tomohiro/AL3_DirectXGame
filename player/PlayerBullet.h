#include "Model.h"
#include "WorldTransform.h"
#include "collider/Collider.h"
#pragma once
class PlayerBullet : public Collider
{
public:
	static const int32_t kLifeTime = 60 * 5;
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();
	void OnCollision() override;
	Vector3 GetWorldPosition() override;
	void Draw(const ViewProjection& viewProjection);
	bool isDead() const { return isDead_; };
	float GetRadius() { return radius_; };

private:
	//ワールド座標データ
	WorldTransform worldTransform_;
	//モデルのポインタ
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//速度
	Vector3 velocity_ = {0, 0, 0};
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
	float radius_ = 1.0f;
};