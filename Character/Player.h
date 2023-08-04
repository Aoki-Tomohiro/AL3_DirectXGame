#pragma once
#include "BaseCharacter.h"
#include "Input.h"
#include <optional>

/// <summary>
/// 自キャラ
/// </summary>
class Player : public BaseCharacter {
public:
	//振るまい
	enum class Behavior {
		kRoot,   // 通常状態
		kAttack, // 攻撃中
	};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="textureHandle"></param>
	void Initialize(const std::vector<Model*>& models) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection) override;

	/// <summary>
	/// 通行行動初期化
	/// </summary>
	void BehaviorRootInitialize();

	/// <summary>
	/// 通常行動更新
	/// </summary>
	void BehaviorRootUpdate();

	/// <summary>
	/// 攻撃行動初期化
	/// </summary>
	void BehaviorAttackInitialize();

	/// <summary>
	/// 攻撃行動更新
	/// </summary>
	void BehaviorAttackUpdate();

	/// <summary>
	/// 浮遊ギミックの初期化
	/// </summary>
	void InitializeFloatingGimmick();

	/// <summary>
	/// 浮遊ギミック更新
	/// </summary>
	void UpdateFloatingGimmick();

	/// <summary>
	/// ビュープロジェクションをセット
	/// </summary>
	/// <param name="viewProjection"></param>
	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

public:
	//ワールドトランスフォーム
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	WorldTransform worldTransformWeapon_;
	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	//浮遊移動のサイクル<frame>
	uint16_t cycle_ = 60;
	//浮遊の振動<m>
	float amplitude_ = 0.1f;
	//振るまい
	Behavior behavior_ = Behavior::kRoot;
	//次の振るまいのリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	//攻撃用のタイマー
	uint16_t animationTimer_ = 0;
	uint16_t animationCount_ = 0;
};