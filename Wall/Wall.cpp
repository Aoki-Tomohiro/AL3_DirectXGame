#include "Wall.h"
#include <cassert>

void Wall::Initialize(Model* model, const Vector3& position, const Vector3& rotation) { 
	//モデルの初期化
	assert(model);
	model_ = model;
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;
	//worldTransform_.scale_ = {1.0f, 0.1f, 1.0f};
}

void Wall::Update() {
	//行列の更新
	worldTransform_.UpdateMatrix();
}

void Wall::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection);
}