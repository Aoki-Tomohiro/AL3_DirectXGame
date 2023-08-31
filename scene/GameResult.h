#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Character/Player.h"
#include "Score/Score.h"
#include <memory>
#include <vector>

/// <summary>
/// リザルトシーン
/// </summary>
class GameResult{

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameResult();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameResult();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// シーン切り替えのフラグを取得
	/// </summary>
	/// <returns></returns>
	bool GetChangeFlag() { return isChange_; };

	/// <summary>
	/// 次のシーン番号を取得
	/// </summary>
	/// <returns></returns>
	int32_t GetNextScene() { return nextScene_; };

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	//テクスチャ
	uint32_t textureHandleNum_[10]{};
	uint32_t textureHandleScore_ = 0;
	uint32_t textureHandleKills_ = 0;
	uint32_t textureHandleLife_ = 0;

	//キル数のスプライト
	std::unique_ptr<Sprite> spriteKills_{};
	std::array<std::unique_ptr<Sprite>, 3> spriteKillsNum_{};
	int32_t eachNumberKills_[3]{};

	//残りライフのスプライト
	std::unique_ptr<Sprite> spriteLife_{};
	std::unique_ptr<Sprite> spriteLifeNum_{};

	//スコアのスプライト
	std::unique_ptr<Sprite> spriteScore_ = nullptr;
	std::array<std::unique_ptr<Sprite>, 5> spriteNumScore_{};
	int32_t eachNumberScore_[5]{};

	//ブラックアウト用のスプライト
	uint32_t textureHandleResult_ = 0;
	std::unique_ptr<Sprite> spriteResult_ = nullptr;

	//スコア
	std::unique_ptr<Score> score_ = nullptr;

	//プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	// シーン切り替えのフラグ
	bool isChange_ = false;
	int32_t nextScene_ = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
