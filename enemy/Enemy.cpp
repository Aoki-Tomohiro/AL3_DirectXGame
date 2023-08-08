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

void Enemy::ChangeState(IEnemyState* newState) { 
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

	//EnemyStateの更新
	state_->Update(this);

	//行列の更新
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}