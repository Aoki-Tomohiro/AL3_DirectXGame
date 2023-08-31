#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <memory>

/// <summary>
/// タイトルシーン
/// </summary>
class GameTitle {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameTitle();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameTitle();

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
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// 追従対象
	const WorldTransform* target_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandleTitle_ = 0;
	//スプライト
	std::unique_ptr<Sprite> spriteTitle_ = nullptr;

	//壁
	// ワールド変換データ
	WorldTransform worldTransformWall_[4]{};
	// モデル
	std::unique_ptr<Model> modelWall_ = nullptr;

	//地面
	//ワールド変換データ
	WorldTransform worldTransformGround_;
	//モデル
	std::unique_ptr<Model> modelGround_ = nullptr;

	//自キャラ
	//ワールド変換データ
	WorldTransform worldTransform_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	// モデルデータ配列
	std::unique_ptr<Model> modelFighterBody_ = nullptr;
	std::unique_ptr<Model> modelFighterL_arm_ = nullptr;
	std::unique_ptr<Model> modelFighterR_arm_ = nullptr;

	//ゲーム説明のテクスチャ
	bool guideFlag_ = false;
	int32_t guideTimer_ = 0;
	uint32_t textureHandleGuide_ = 0;
	std::unique_ptr<Sprite> spriteGuide_ = nullptr;

	//シーン切り替えのフラグ
	bool isChange_ = false;
	int32_t nextScene_ = 0;

	//BGM
	bool isPlayAudio_ = false;
	uint32_t soundDataHandleTitle_ = 0;
	uint32_t voiceHandleTitle_ = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
