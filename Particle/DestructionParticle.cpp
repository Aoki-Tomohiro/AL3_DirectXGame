#include "DestructionParticle.h"
#include "MathFunction.h"
#include <cstdlib>
#include <ctime>
#include <cassert>

void DestructionParticle::Initialize(Model* model, uint32_t textureHandle, const Vector3& position){
	//モデルの初期化
	assert(model); 
	model_ = model;
	//テクスチャハンドルの初期化
	textureHandle_ = textureHandle;
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	//角度の初期化
	worldTransform_.rotation_.x = float(rand() % 628 - 314) / 100;
	worldTransform_.rotation_.y = float(rand() % 628 - 314) / 100;
	worldTransform_.rotation_.z = float(rand() % 628 - 314) / 100;
	//スケールの初期化
	worldTransform_.scale_.x = 0.1f;
	worldTransform_.scale_.y = 0.1f;
	worldTransform_.scale_.z = 0.1f;
	//速度の初期化
	velocity_.x = float(rand() % 20 - 10) / 100;
	velocity_.y = float(rand() % 30) / 100;
	velocity_.z = float(rand() % 20 - 10) / 100;
}

void DestructionParticle::Update() {
	//アクティブフラグをtrueにする
	if (isActive_ == false) {
		isActive_ = true;
	}

	if (isActive_) {
		// 毎フレーム速度を落とす
		velocity_.y -= 0.01f;

		// 移動処理
		worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
		// 行列の更新
		worldTransform_.UpdateMatrix();

		// パーティクルが地面より下に移動したら消す
		if (worldTransform_.translation_.y <= 0) {
			isActive_ = false;
			isDead_ = true;
		}
	}
}

void DestructionParticle::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	if (isActive_) {
		model_->Draw(worldTransform_, viewProjection, textureHandle_);
	}
}