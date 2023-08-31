#include "HitParticle.h"
#include "MathFunction.h"
#include <cstdlib>
#include <ctime>
#include <cassert>

void HitParticle::Initialize(Model* model, const Vector3& position) {
	// モデルの初期化
	assert(model);
	model_ = model;
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.translation_.y = 1.0f;
	//速度の初期化
	velocity_.x = float(rand() % 101 - 50) / 100;
	velocity_.y = float(rand() % 101 - 50) / 100;
	velocity_.z = float(rand() % 101 - 50) / 100;
	//移動方向に向きを合わせる
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	float length = Length(Vector3{velocity_.x, 0, velocity_.z});
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, length);
	//スケールをランダムに決める
	worldTransform_.scale_.z = float(rand() % 6 + 5) / 10;
}

void HitParticle::Update() {
	// アクティブフラグをtrueにする
	if (isActive_ == false) {
		isActive_ = true;
		activeTimer_ = kActiveTime;
	}

	if (isActive_) {
		//アクティブタイマーをデクリメント
		activeTimer_--;

		//移動処理
		worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

		//徐々に小さくする
		if (worldTransform_.scale_.z > 0) {
			worldTransform_.scale_.z -= 0.05f;
		}

		// 行列の更新
		worldTransform_.UpdateMatrix();

		// パーティクルが地面より下に移動したら消す
		if (activeTimer_ <= 0) {
			isActive_ = false;
			isDead_ = true;
		}
	}
}

void HitParticle::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	if (isActive_) {
		model_->Draw(worldTransform_, viewProjection);
	}
}