#pragma once
#include "WorldTransform.h"
#include "IEnemyState.h"
#include "EnemyStateApproach.h"
#include "EnemyStateLeave.h"
#include "Model.h"
#include <stdio.h>

enum class Phase
{
	Approach,//接近する
	Leave,//離脱する
};

class Enemy {
public:
	void Initialize(Model* model, uint32_t textureHandle);
	void Approach();
	void Leave();
	void Update();
	void Draw(ViewProjection viewProjection);
	void ChangeState(IEnemyState* newState);
	WorldTransform GetWorldTransform() { return worldTransform_; };
	void EnemyMove(Vector3 move);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//フェーズ
	Phase phase_ = Phase::Approach;
	//メンバ関数ポインタのテーブル
	static void(Enemy::*spFuncTable[])();
	//statePattern
	IEnemyState* state_;
};