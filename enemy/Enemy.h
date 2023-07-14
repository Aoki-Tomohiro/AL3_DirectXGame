#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "EnemyBullet.h"
#include "TimeCall.h"
#include <stdio.h>
#include "collider/Collider.h"
#include "IEnemyState.h"
#include "EnemyStateApproach.h"
#include "EnemyStateLeave.h"

//GameSceneの前方宣言(苦肉の策)
class GameScene;
//Playerの前方宣言
class Player;

//class Enemy;
//
//class IEnemyState {
//public:
//	virtual void Initialize(Enemy* pEnemy) = 0;
//	virtual void Update() = 0;
//
//protected:
//	Enemy* enemy_ = nullptr;
//};

//class EnemyStateApproach : public IEnemyState {
//public:
//	void Initialize(Enemy* pEnemy);
//	void Update();
//};

//class EnemyStateLeave : public IEnemyState {
//public:
//	void Initialize(Enemy* pEnemy);
//	void Update();
//};

class Enemy : public Collider{
public:
	static const int kFireInterval = 60;
	~Enemy();
	void Initialize(Model* model, uint32_t textureHandle, const Vector3& pos);
	void Update();
	void OnCollision() override;
	Vector3 GetWorldPosition() override;
	void Fire();
	void FireReset();
	void Draw(ViewProjection viewProjection);
	void ChangeState(IEnemyState* newState);
	WorldTransform GetWorldTransform() { return worldTransform_; };
	void EnemyMove(Vector3 move);
	int32_t GetFireTimer() { return fireTimer_; };
	void SetFireTimer(int32_t fireTimer) { this->fireTimer_ = fireTimer; };
	std::list<TimedCall*> GetTimedCall() { return timedCalls_; };
	void SetPlayer(Player* player) { player_ = player; };
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; };
	bool isDead() const { return isDead_; };

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//statePattern
	IEnemyState* state_;
	////弾
	//発射タイマー
	int32_t fireTimer_ = kFireInterval;
	//時限発動のリスト
	std::list<TimedCall*> timedCalls_;
	//生存フラグ
	bool isDead_ = false;

	//自キャラ
	Player* player_ = nullptr;
	//ゲームシーン
	GameScene* gameScene_ = nullptr;
};