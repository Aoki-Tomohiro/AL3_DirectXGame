#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "EnemyBullet.h"
#include "TimeCall.h"
#include <stdio.h>

class Player;

enum class Phase
{
	Approach,//接近する
	Leave,//離脱する
};

class Enemy;

class BaseEnemyState {
public:
	virtual void Initialize(Enemy* pEnemy) = 0;
	virtual void Update(Enemy* pEnemy) = 0;
};

class EnemyStateApproach : public BaseEnemyState {
public:
	void Initialize(Enemy* pEnemy);
	void Update(Enemy* pEnemy);
};

class EnemyStateLeave : public BaseEnemyState {
public:
	void Initialize(Enemy* pEnemy);
	void Update(Enemy* pEnemy);
};

class Enemy {
public:
	static const int kFireInterval = 60;
	~Enemy();
	void Initialize(Model* model, uint32_t textureHandle);
	void Update();
	void Fire();
	void FireReset();
	void Draw(ViewProjection viewProjection);
	void ChangeState(BaseEnemyState* newState);
	float GetEnemySpeed() { return enemySpeed_; };
	WorldTransform GetWorldTransform() { return worldTransform_; };
	void EnemyMove(Vector3 move);
	std::list<std::unique_ptr<EnemyBullet>> &GetEnemyBullet() {	return bullets_;}
	int32_t GetFireTimer() { return fireTimer_; };
	void SetFireTimer(int32_t fireTimer) { this->fireTimer_ = fireTimer; };
	std::list<TimedCall*> GetTimedCall() { return timedCalls_; };
	void SetPlayer(Player* player) { player_ = player; };
	Vector3 GetWorldPosition();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//フェーズ
	Phase phase_ = Phase::Approach;
	//敵の速度
	float enemySpeed_ = 0.0f;
	//txtファイル
	FILE* fp;
	errno_t err_;
	//statePattern
	BaseEnemyState* state_;
	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//発射タイマー
	int32_t fireTimer_ = kFireInterval;
	//時限発動のリスト
	std::list<TimedCall*> timedCalls_;

	//自キャラ
	Player* player_ = nullptr;
};