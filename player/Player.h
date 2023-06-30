#include <Model.h>
#include <WorldTransform.h>
#include <Input.h>
#include "PlayerBullet.h"
#include <list>

#pragma once
class Player {
public:
	~Player();
	void Initialize(Model* model, uint32_t textureHandle);
	void Update();
	void OnCollision();
	void Draw(ViewProjection viewProjection);
	Vector3 GetWorldPosition();
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; };
	float GetRadius() { return radius_; };

private:
	void Attack();
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//キーボード入力
	Input* input_ = nullptr;
	//ImGuiで値を入力する変数
	float* inputFloat3[3] = {
	    &worldTransform_.translation_.x, &worldTransform_.translation_.y,
	    &worldTransform_.translation_.z};
	//弾
	std::list<PlayerBullet*> bullets_;
	float radius_ = 1.0f;
};