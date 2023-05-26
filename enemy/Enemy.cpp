#include "Enemy.h"
#include <cassert>
#include "ImGuiManager.h"
#include "MathFunction.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = {10.0f, 0.0f, 50.0f};
	char fname[] = "enemy/enemySpeed.txt";
	err_ = fopen_s(&fp, fname, "r");
	if (fp != 0  && err_ == 0)
	{
		fscanf_s(fp, "%f", &enemySpeed_);
		fclose(fp);
	}
	state_ = new EnemyStateApproach();
}

void Enemy::ChangeState(BaseEnemyState* newState) { 
	state_ = newState;
}

void Enemy::EnemyMove(Vector3 move) {
	Move(worldTransform_.translation_, move);
}

void Enemy::Update() {

	state_->Update(this);

	//行列の更新
	worldTransform_.UpdateMatrix();
}

void Enemy::Fire() {
	//座標のコピー
	Vector3 position = worldTransform_.translation_;

	//弾の生成
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, position);

	//弾を登録する
	bullets_.push_back(std::unique_ptr<EnemyBullet>(newBullet));
}

void Enemy::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// 弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void EnemyStateApproach::Initialize(Enemy* pEnemy) { 
	pEnemy->SetFireTimer(pEnemy->kFireInterval);
}

void EnemyStateApproach::Update(Enemy* pEnemy) {
	// 移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラクターの移動速さ
	 const float kEnemySpeed = pEnemy->GetEnemySpeed();

	// 移動(ベクトルを加算)
	 move.z -= kEnemySpeed;
	// 座標移動(ベクトルの加算)
	 pEnemy->EnemyMove(move);
	// 既定の位置に到達したら離脱
	if (pEnemy->GetWorldTransform().translation_.z < 0.0f) {
		pEnemy->ChangeState(new EnemyStateLeave());
	}

	//弾の発射
	pEnemy->SetFireTimer(pEnemy->GetFireTimer() - 1);
	if (pEnemy->GetFireTimer() < 0) {
		pEnemy->Fire();
		pEnemy->SetFireTimer(pEnemy->kFireInterval);
	}

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : pEnemy->GetEnemyBullet()) {
		bullet->Update();
	}
}

void EnemyStateLeave::Initialize(Enemy* pEnemy) { 
	pEnemy->SetFireTimer(pEnemy->kFireInterval);
}

void EnemyStateLeave::Update(Enemy* pEnemy) {
	// 移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラクターの移動速さ
	const float kEnemySpeed = pEnemy->GetEnemySpeed();

	// 移動(ベクトルを加算)
	move.x -= kEnemySpeed;
	move.y += kEnemySpeed;
	move.z -= kEnemySpeed;
	// 移動(ベクトルを加算)
	pEnemy->EnemyMove(move);

	// 弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : pEnemy->GetEnemyBullet()) {
		bullet->Update();
	}
}