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
}

void Enemy::Approach() {
	// 移動ベクトル
	Vector3 move = {0, 0, 0};
	//// キャラクターの移動速さ
	//const float kEnemySpeed = 0.2f;

	// 移動(ベクトルを加算)
	move.z -= enemySpeed_;
	// 座標移動(ベクトルの加算)
	Move(worldTransform_.translation_, move);
	// 既定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}
void Enemy::Leave(){
	// 移動ベクトル
	Vector3 move = {0, 0, 0};
	//// キャラクターの移動速さ
	//const float kEnemySpeed = 0.1f;

	// 移動(ベクトルを加算)
	move.x -= enemySpeed_;
	move.y += enemySpeed_;
	move.z -= enemySpeed_;
	// 移動(ベクトルを加算)
	Move(worldTransform_.translation_, move);
}

void (Enemy::*Enemy::spFuncTable[])() = {
	&Enemy::Approach,
	&Enemy::Leave
};

void Enemy::Update() {

	/*switch (phase_) {
	case Phase::Approach:
	default:
		Enemy::Approach();
		break;
	case Phase::Leave:
		Enemy::Leave();
		break;
	}*/
	(this->*spFuncTable[0])();
	(this->*spFuncTable[1])();

	//行列の更新
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}