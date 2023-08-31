#pragma once
#include "Model.h"
#include "WorldTransform.h"

/// <summary>
/// ヒットパーティクル
/// </summary>
class HitParticle {
public:
	//パーティクルが消えるまでの時間
	const int32_t kActiveTime = 10;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="position"></param>
	void Initialize(Model* model, const Vector3& position);

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
	//速度
	Vector3 velocity_{0.0f, 0.0f, 0.0f};
	//アクティブフラグ
	bool isActive_ = false;
	//アクティブタイマー
	int32_t activeTimer_ = 0;
	//デスフラグ
	bool isDead_ = false;
};