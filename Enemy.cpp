#include "Enemy.h"
#include <cassert>
#include "ImGuiManager.h"
#include "MathFunction.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 2.0f, 50.0f};
}

void Enemy::Update() {
	
	//移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラクターの移動速さ
	const float kEnemySpeed = 0.2f;

	//移動処理
	move.z -= kEnemySpeed;

	//座標移動(ベクトルの加算)
	Move(worldTransform_.translation_, move);

	//行列の更新
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}