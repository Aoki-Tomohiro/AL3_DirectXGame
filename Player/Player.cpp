#include "Player.h"
#include "math/MathFunction.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
}

void Player::Update() { 
	//行列の更新
	worldTransform_.UpdateMatrix(); 
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}