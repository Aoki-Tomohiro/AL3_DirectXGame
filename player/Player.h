#include <Model.h>
#include <WorldTransform.h>
#include <Input.h>
#include "PlayerBullet.h"
#include <list>
#include "collider/Collider.h"
#include"Sprite.h"
#pragma once

class Enemy;

class Player : public Collider{
public:
	~Player();
	void Initialize(Model* model, uint32_t textureHandle, Vector3& playerPosition,uint32_t reticleTextureHandle);
	void Update(const ViewProjection& viewProjection);
	void OnCollision() override;
	Vector3 GetWorldPosition() override;
	void Draw(ViewProjection viewProjection);
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() const { return bullets_; };
	void SetParent(const WorldTransform* parent);
	const WorldTransform& GetWorldTransform() { return worldTransform_; };
	void Set3DReticleWorldPosition(const Vector3& worldPos);
	Vector3 Get3DReticleWorldPosition();
	void DrawUI();
	void SetEnemy(const std::list<Enemy*> enemys) { enemys_ = enemys; };

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
	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;
	//2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;
	//敵キャラ
	std::list<Enemy*> enemys_;
	std::list<Vector3> enemyPos_;
};