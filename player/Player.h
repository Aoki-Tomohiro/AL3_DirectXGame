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
	void Draw(ViewProjection viewProjection);
	Vector3 GetWorldPosition() {
		//ワールド座標を入れる変数
		Vector3 worldPos;
		//ワールド行列の平行移動成分を取得(ワールド座標)
		worldPos.x = worldTransform_.translation_.x;
		worldPos.y = worldTransform_.translation_.y;
		worldPos.z = worldTransform_.translation_.z;
		return worldPos;
	};

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
};