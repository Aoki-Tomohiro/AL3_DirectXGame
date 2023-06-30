#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Player;

class EnemyBullet {
public:
	static const int32_t kLifeTime = 30;
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void OnCollision();
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void SetPlayer(Player* player) { player_ = player; };
	bool isDead() const { return isDead_; };
	Vector3 GetWorldPosition();
	float GetRadius() { return radius_; };

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_;
	Player* player_;
	float t_ = 0.0f;
	bool isDead_ = false;
	float radius_ = 1.0f;
};