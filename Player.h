#include <Model.h>
#include <WorldTransform.h>
#include <Input.h>
#include "PlayerBullet.h"

#pragma once
class Player {
public:
	void Initialize(Model* model, uint32_t textureHandle);
	void Update();
	void Draw(ViewProjection viewProjection);
	void Attack();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//キーボード入力
	Input* input_ = nullptr;
	// ImGuiで値を入力する変数
	float* inputFloat3[3] = {
	    &worldTransform_.translation_.x, &worldTransform_.translation_.y,
	    &worldTransform_.translation_.z};
	//弾
	PlayerBullet* bullet_ = nullptr;
};