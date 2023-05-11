#include "Model.h"
#include "WorldTransform.h"
#pragma once
class PlayerBullet
{
public:
	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);

private:
	//ワールド座標データ
	WorldTransform worldTransform_;
	//モデルのポインタ
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};