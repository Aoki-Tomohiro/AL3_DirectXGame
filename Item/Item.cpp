#include "Item.h"
#include <cassert>

void Item::Initialize(Model* model,uint32_t textureHandle) {
	//モデルの初期化
	assert(model);
	model_ = model;
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_.x = float(rand() % 201 - 100);
	worldTransform_.translation_.y = 0.5f;
	worldTransform_.translation_.z = float(rand() % 201 - 100);
	worldTransform_.scale_ = {0.5f, 0.5f, 0.5f};
	//テクスチャを取得
	textureHandle_ = textureHandle;
	//半径を設定
	SetRadius(0.5f);
}

void Item::Update() {
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	//行列の更新
	worldTransform_.UpdateMatrix();
}

void Item::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection,textureHandle_);
}

void Item::OnCollision() { 
	isDead_ = true; 
}

Vector3 Item::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}