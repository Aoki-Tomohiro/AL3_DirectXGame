#include "EnemyBullet.h"
#include "MathFunction.h"
#include <cassert>
#include "player/Player.h"
#include"ImGuiManager.h"
#include "collider/CollisionConfig.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position,const Vector3& velocity) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("enemyBullet.png");
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;
	//スピードの初期化
	velocity_ = velocity;
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	float length = Length(Vector3{velocity.x, 0, velocity.z});
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, length);
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributeEnemy);
}

void EnemyBullet::OnCollision() { 
	isDead_ = true;
}

void EnemyBullet::Update() {
	SetPlayer(player_);

	//敵弾から自キャラへのベクトルを計算
	Vector3 toPlayer = Subtract(player_->GetWorldPosition(), worldTransform_.translation_);

	if (t_ < 0.05) {
		t_ += 1.0f / 120.0f;
	}

	//ベクトルを正規化する
	Vector3 nPlayer = Normalize(toPlayer);
	Vector3 nVelocity = Normalize(velocity_);
	velocity_ = Slerp(nVelocity, nPlayer, t_);
	velocity_.x *= 0.5f;
	velocity_.y *= 0.5f;
	velocity_.z *= 0.5f;

	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	float length = Length(Vector3{velocity_.x, 0, velocity_.z});
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, length);

	Move(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 EnemyBullet::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
};