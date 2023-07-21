#include "Ground.h"
#include <cassert>

void Ground::Initialize(Model* model) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
};

void Ground::Update() {
	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
};

void Ground::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
};