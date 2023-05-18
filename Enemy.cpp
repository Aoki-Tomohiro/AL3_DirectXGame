#include "Enemy.h"
#include <cassert>
#include "ImGuiManager.h"
#include "MathFunction.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 2.0f, 50.0f};
	char fname[] = "enemySpeed.txt";
	err_ = fopen_s(&fp, fname, "r");
	if (fp != 0  && err_ == 0)
	{
		fscanf_s(fp, "%f", &enemySpeed_);
		fclose(fp);
	}
	spFuncTable[0] = &Enemy::Approach;
	spFuncTable[1] = &Enemy::Leave;
	state_ = new EnemyStateApproach();
}

void Enemy::Approach() {
	state_->Update(this);
}

void Enemy::Leave(){
	state_->Update(this);
}

void Enemy::ChangeState(BaseEnemyState* newState) { 
	state_ = newState;
}

void Enemy::EnemyMove(Vector3 move) {
	Move(worldTransform_.translation_, move);
}

void (Enemy::*Enemy::spFuncTable[])() = {
	&Enemy::Approach,
	&Enemy::Leave
};

void Enemy::Update() {

	//メンバ関数ポインタの呼び出し
	(this->*spFuncTable[0])();
	(this->*spFuncTable[1])();

	//行列の更新
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
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
}