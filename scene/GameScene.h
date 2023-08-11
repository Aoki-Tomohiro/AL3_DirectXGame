#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "collider/CollisionManager.h"
#include "enemy/Enemy.h"
#include "player/Player.h"
#include "skydome/Skydome.h"
#include "RailCamera/RailCamera.h"
#include <sstream>

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

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet"></param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	/// <summary>
	/// 敵生成
	/// </summary>
	void AddEnemy(const Vector3& pos);

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
	std::list<Enemy*> enemys_;
	//敵発生コマンド
	std::stringstream enemyPopCommands;
	// 衝突マネージャー
	CollisionManager* collisionManager_ = nullptr;
	// 3Dモデル
	Model* modelSkydome_ = nullptr;
	// skydome
	std::unique_ptr<Skydome> skydome_;
	//RailCamera
	RailCamera* railCamera_ = nullptr;
	// 弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;
	//待機中フラグ
	bool waitingFlag;
	uint32_t waitTimer;
	//レティクル用テクスチャ
	uint32_t textureReticle_ = 0;
	//ロックオン時のレティクル用テクスチャ
	uint32_t lockonTexture_ = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
