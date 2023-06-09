#include "EnemyBullet.h"
#include "MathFunction.h"
#include <cassert>

void EnemyBullet::Initialize(Model* model, const Vector3& position,const Vector3& velocity) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("enemyBullet.png");
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;
	//スピードの初期化
	velocity_ = velocity;
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	float length = Length(Vector3{velocity.x, 0, velocity.z});
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, length);
}

void EnemyBullet::Update() {
	Move(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}