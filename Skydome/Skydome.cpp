#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
};

void Skydome::Update() {
	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
};

void Skydome::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
};