#pragma once
#include "WorldTransform.h"
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
};