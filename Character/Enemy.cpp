#include "Enemy.h"
#include "MathFunction.h"

void Enemy::Initialize(const std::vector<Model*>& models) {
	BaseCharacter::Initialize(models);
	worldTransform_.translation_.z = 20;
	//ワールドトランスフォームの初期化
	worldTransformBody_.Initialize();
	//親子関係
	worldTransformBody_.parent_ = &worldTransform_;
}

void Enemy::Update() { 
	//回転
	const float rotSpeed = 0.08f;
	worldTransform_.rotation_.y += rotSpeed;
	//回転方向に移動
	Vector3 velocity{0.0f, 0.0f, 0.2f};
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);
	//移動処理
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity);

	//ワールドトランスフォームの更新
	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	//3Dモデルを描画
	models_[0]->Draw(worldTransformBody_, viewProjection);
}