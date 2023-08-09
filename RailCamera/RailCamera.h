#pragma once
#include <vector>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "math/MathFunction.h"

class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3 translation, Vector3 rotation);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	/// <summary>
	/// ワールドトランスフォームを取得
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_; };

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	// スプライン曲線制御点(通過点)
	std::vector<Vector3> controlPoints_;
	Vector3 eye_;
	Vector3 target_;
	float eyet_ = 0.0f;
	float targett_ = 0.1f;
	uint32_t eyeSection_ = 0;
	uint32_t targetSection_ = 0;
};