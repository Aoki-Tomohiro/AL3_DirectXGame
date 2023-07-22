#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "AxisIndicator.h"
#include "Player/Player.h"
#include "Skydome/Skydome.h"
#include "Ground/Ground.h"
#include <memory>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	//自キャラ
	std::unique_ptr<Model> modelPlayer_ = nullptr;
	std::unique_ptr<Player> player_ = nullptr;
	//天球
	std::unique_ptr<Model> modelSkydome_ = nullptr;
	std::unique_ptr<Skydome> skydome_ = nullptr;
	//地面
	std::unique_ptr<Model> modelGround_ = nullptr;
	std::unique_ptr<Ground> ground_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
