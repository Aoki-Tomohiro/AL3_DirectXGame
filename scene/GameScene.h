#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "RailCamera/RailCamera.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "collider/CollisionManager.h"
#include "enemy/Enemy.h"
#include "player/Player.h"
#include "skydome/Skydome.h"

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
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 3Dモデルのデータ
	Model* model_ = nullptr;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// 自キャラ
	Player* player_ = nullptr;
	// デバッグカメラの有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	// 敵
	Enemy* enemy_ = nullptr;
	// 衝突マネージャー
	CollisionManager* collisionManager_ = nullptr;
	// 3Dモデル
	Model* modelSkydome_ = nullptr;
	// skydome
	std::unique_ptr<Skydome> skydome_;
	// RailCamera
	RailCamera* railCamera_ = nullptr;
	// スプライン曲線制御点(通過点)
	std::vector<Vector3> controlPoints_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
