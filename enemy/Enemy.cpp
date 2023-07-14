#include "GameScene.h"
#include "Enemy.h"
#include <cassert>
#include "ImGuiManager.h"
#include "MathFunction.h"
#include "player/Player.h"
#include "collider/CollisionConfig.h"

Enemy::~Enemy() {
	for (TimedCall* timedCall : timedCalls_) {
		delete timedCall;
	}
}

void Enemy::Initialize(Model* model, uint32_t textureHandle,const Vector3& pos) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	state_ = new EnemyStateApproach();
	state_->Initialize(this);
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributeEnemy);
	SetRadius(1.0f);
}

void Enemy::ChangeState(IEnemyState* newState) { 
	state_ = nullptr;
	state_ = newState;
}

void Enemy::EnemyMove(Vector3 move) {
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
}

void Enemy::OnCollision(){ 
	isDead_ = true; 
};

void Enemy::Update() {
	// 弾の削除
	timedCalls_.remove_if([](TimedCall* timedCall) {
		if (timedCall->IsFinished() == true) {
			delete timedCall;
			return true;
		}
		return false;
	});

	state_->Update(this);

	//行列の更新
	worldTransform_.UpdateMatrix();
}

void Enemy::Fire() {
	assert(player_);

	//弾の速度
	const float kBulletSpeed = 0.5f;

	//自キャラのワールド座標を取得する
	Vector3 playerPos = player_->GetWorldPosition();
	//敵キャラのワールド座標を取得する
	Vector3 enemyPos = worldTransform_.translation_;
	Vector3 targetPos = Subtract(playerPos,enemyPos);
	Vector3 n = Normalize(targetPos);
	Vector3 velocity = {n.x * kBulletSpeed, n.y * kBulletSpeed, n.z * kBulletSpeed};

	//弾の生成
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_,velocity);
	newBullet->SetPlayer(player_);

	//弾を登録する
	gameScene_->AddEnemyBullet(newBullet);
}

void Enemy::FireReset() {
	Fire();
	// メンバ関数と呼び出し元をbindしてstd::functionに代入
	std::function<void(void)> callback = std::bind(&Enemy::FireReset, this);
	//時限発動イベントを生成
	TimedCall* timedCall = new TimedCall(callback, 60);
	//時限発動イベントを時限発動イベントリストに追加
	timedCalls_.push_back(timedCall);
}

void Enemy::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
};