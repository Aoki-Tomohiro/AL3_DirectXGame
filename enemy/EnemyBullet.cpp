#include "EnemyBullet.h"
#include "MathFunction.h"
#include <cassert>

void EnemyBullet::Initialize(Model* model, const Vector3& position) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("enemyBullet.png");
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void EnemyBullet::Update() {
	Vector3 move = {0, 0, 0};
	const float kBulletSpeed = 0.5f;
	move.z -= kBulletSpeed;
	Move(worldTransform_.translation_, move);
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}