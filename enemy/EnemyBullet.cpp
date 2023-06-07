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
	//スピードの初期化
	velocity_ = velocity;
}

void EnemyBullet::Update() {
	Move(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}