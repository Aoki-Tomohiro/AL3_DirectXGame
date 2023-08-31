#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "CollisionManager/Collider.h"

/// <summary>
/// アイテム
/// </summary>
class Item : public Collider{
public:
	//アイテムが消える時間
	static const int32_t kLifeTime = 60 * 30;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model_"></param>
	void Initialize(Model* model_, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 衝突判定
	/// </summary>
	void OnCollision() override;

	/// <summary>
	/// ワールドポジションを取得
	/// </summary>
	Vector3 GetWorldPosition() override;

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
	//デスフラグ
	bool isDead_ = false;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
};