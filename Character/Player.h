#pragma once
#include "BaseCharacter.h"
#include "Input.h"
#include "Sprite.h"
#include "CollisionManager/Collider.h"
#include <vector>
#include <memory>
#include <optional>

//前方宣言
class GameScene;

/// <summary>
/// 自キャラ
/// </summary>
class Player : public BaseCharacter, public Collider{
public:
	//ライフの最大数
	static const int32_t kLifeMax = 3;
	//シェイクの時間
	static const int32_t kShakeTime = 30;
	//無敵時間
	static const int32_t kInvincibleTime = 60;
	//強化時間
	static const int32_t kStrongTime = 600;

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
	void Initialize(
	    const std::vector<Model*>& models, const std::vector<uint32_t*>& textureHandles,
	    int32_t level) override;

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
	/// UI描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// 衝突判定
	/// </summary>
	void OnCollision() override;

	/// <summary>
	/// ワールドポジションを取得
	/// </summary>
	Vector3 GetWorldPosition() override;

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
	/// 射撃
	/// </summary>
	void Fire();

	/// <summary>
	/// 浮遊ギミックの初期化
	/// </summary>
	void InitializeFloatingGimmick();

	/// <summary>
	/// 浮遊ギミック更新
	/// </summary>
	void UpdateFloatingGimmick();

	/// <summary>
	/// シェイク
	/// </summary>
	void ShakeLifeTexture();

	/// <summary>
	/// 強化状態にする
	/// </summary>
	void SetStrongState();

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyGlobalVariables();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	void LifeReset() { // 体力を初期化
		sLife_ = kLifeMax;
	};

	/// <summary>
	/// 体力をリセット
	/// </summary>
	void ResetIsDead() { isDead_ = false; };

	/// <summary>
	/// 無敵時間をリセット
	/// </summary>
	void ResetInvincibleTimer() { invincibleTimer_ = 0; };

	/// <summary>
	/// ビュープロジェクションをセット
	/// </summary>
	/// <param name="viewProjection"></param>
	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	/// <summary>
	/// ゲームシーンをセット
	/// </summary>
	/// <param name="gameScene"></param>
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; };

	/// <summary>
	/// 射撃サウンドデータハンドルをセット
	/// </summary>
	/// <param name="soundDataHandle"></param>
	void SetSoundDataHandleShot(uint32_t soundDataHandle) { soundDataHandleShot_ = soundDataHandle; };

	/// <summary>
	/// ダメージサウンドデータハンドルをセット
	/// </summary>
	/// <param name="soundDataHandle"></param>
	void SetSoundDataHandleDamage(uint32_t soundDataHandle) {
		soundDataHandleDamage_ = soundDataHandle;
	};

	/// <summary>
	/// ライフを取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetLife() { return sLife_; };

public:
	//ワールドトランスフォーム
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	WorldTransform worldTransformWeapon_;
	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	//弾の発射タイマー
	int32_t bulletTime;
	int32_t bulletTimer_ = 0;
	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	//浮遊移動のサイクル<frame>
	int32_t cycle_ = 60;
	//浮遊の振動<m>
	float amplitude_ = 0.1f;
	//振るまい
	Behavior behavior_ = Behavior::kRoot;
	//次の振るまいのリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	//ゲームシーン
	GameScene* gameScene_ = nullptr;
	//ライフ
	std::unique_ptr<Sprite> spriteLifes_[3]{};
	static int32_t sLife_;
	bool isShake_ = false;
	int32_t shakeTimer_ = 0;
	//無敵時間
	bool isInvincible_ = false;
	int32_t invincibleTimer_ = 0;
	//強化状態
	bool isStrong_ = false;
	int32_t strongTimer_ = 0;
	//サウンドデータハンドル
	uint32_t soundDataHandleShot_ = 0;
	uint32_t soundDataHandleDamage_ = 0;
};