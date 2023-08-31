#pragma once
#include "BaseCharacter.h"
#include "CollisionManager/Collider.h"

//ゲームシーンの前方宣言
class GameScene;
//プレイヤーの前方宣言
class Player;
//スコアの前方宣言
class Score;

/// <summary>
/// 敵キャラ
/// </summary>
class Enemy : public BaseCharacter, public Collider{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models"></param>
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
	/// 衝突判定
	/// </summary>
	void OnCollision() override;

	/// <summary>
	/// ワールドポジションを取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() override;

	/// <summary>
	/// プレイヤーをセット
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; };

	/// <summary>
	/// ゲームシーンをセット
	/// </summary>
	/// <param name="gameScene"></param>
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; };

	/// <summary>
	/// スコアをセット
	/// </summary>
	/// <param name="score"></param>
	void SetScore(Score* score) { score_ = score; };

	/// <summary>
	/// サウンドデータハンドルをセット
	/// </summary>
	/// <param name="soundDataHandle"></param>
	void SetSoundDataHandle(uint32_t soundDataHandle) {
		soundDataHandleExplosion_ = soundDataHandle;
	};

private:
	//ワールドトランスフォーム
	WorldTransform worldTransformHead_{};
	WorldTransform worldTransformBody_{};
	WorldTransform worldTransformL_arm_{};
	WorldTransform worldTransformR_arm_{};
	//ゲームシーン
	GameScene* gameScene_ = nullptr;
	//プレイヤー
	Player* player_ = nullptr;
	//スコア
	Score* score_ = nullptr;
	//サウンドデータハンドル
	uint32_t soundDataHandleExplosion_ = 0;
	//ライフ
	int32_t life_ = 0;
};