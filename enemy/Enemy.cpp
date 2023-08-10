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
	char fname[] = "enemy/enemySpeed.txt";
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

	//メンバ関数ポインタの呼び出し
	(this->*spFuncTable[static_cast<size_t>(phase_)])();

	//行列の更新
	worldTransform_.UpdateMatrix();

	ImGui::Begin("Enemy");
	ImGui::Text(
	    "Enemy Pos : (%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	if (phase_ == Phase::Approach) {
		ImGui::Text("Phase Approach");
	} else if (phase_ == Phase::Leave) {
		ImGui::Text("Phase Leave");
	}
	ImGui::End();
}

void Enemy::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}