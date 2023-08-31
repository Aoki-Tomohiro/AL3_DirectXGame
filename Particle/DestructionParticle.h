#pragma once
#include "Model.h"
#include "WorldTransform.h"

/// <summary>
/// 破壊パーティクル
/// </summary>
class DestructionParticle {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// デスフラグを取得
	/// </summary>
	/// <returns></returns>
	bool isDead() const { return isDead_; };

private:
	//モデル
	Model* model_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_{};
	// 速度
	Vector3 velocity_{0.0f, 0.0f, 0.0f};
	//アクティブフラグ
	bool isActive_ = false;
	//デスフラグ
	bool isDead_ = false;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
};