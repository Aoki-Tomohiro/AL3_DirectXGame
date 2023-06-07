#pragma once
#include "Model.h"
#include "WorldTransform.h"

class EnemyBullet {
public:
	static const int32_t kLifeTime = 30;
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_;
};