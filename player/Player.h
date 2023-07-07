#include <Model.h>
#include <WorldTransform.h>
#include <Input.h>
#include "PlayerBullet.h"
#include <list>
#include "collider/Collider.h"

#pragma once
class Player : public Collider{
public:
	~Player();
	void Initialize(Model* model, uint32_t textureHandle);
	void Update();
	void OnCollision() override;
	Vector3 GetWorldPosition() override;
	void Draw(ViewProjection viewProjection);
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() const { return bullets_; };

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
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
};