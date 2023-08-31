#pragma once
#include "Model.h"
#include "WorldTransform.h"

/// <summary>
/// 壁
/// </summary>
class Wall {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="position"></param>
	/// <param name="rotation"></param>
	void Initialize(Model* model, const Vector3& position, const Vector3& rotation);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

private:
	//モデル
	Model* model_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_{};
};