#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_.x = 2;
	worldTransform_.scale_.y = 2;
	worldTransform_.scale_.z = 2;
};

void Skydome::Update() {
	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
};

void Skydome::Draw(ViewProjection& viewProjection){
	model_->Draw(worldTransform_, viewProjection);
};