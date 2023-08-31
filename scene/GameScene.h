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
#include "Character/Player.h"
#include "Character/PlayerBullet.h"
#include "Character/Enemy.h"
#include "Item/Item.h"
#include "Skydome/Skydome.h"
#include "Ground/Ground.h"
#include "Wall/Wall.h"
#include "FollowCamera/FollowCamera.h"
#include "CollisionManager/CollisionManager.h"
#include "Particle/ParticleEmitter.h"
#include "Score/Score.h"
#include "TimeLimit/TimeLimit.h"
#include <memory>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene{

public:
	//敵のリスポーン間隔
	static int32_t enemySpornTime;
	//アイテムのスポーン間隔
	static int32_t itemSpornTime; 

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
	/// 敵を発生させる
	/// </summary>
	void EnemySporn();

	/// <summary>
	/// アイテムを発生させる
	/// </summary>
	void ItemSporn();

	/// <summary>
	/// パーティクルエミッターを作成
	/// </summary>
	/// <param name="particleType"></param>
	/// <param name="position"></param>
	void CreateParticleEmitter(
	    ParticleType particleType, const Vector3& position, int32_t enemyLevel);

	/// <summary>
	/// 自機の弾を追加する
	/// </summary>
	void AddPlayerBullet(PlayerBullet* playerBullet);

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

	//ビュープロジェクション
	ViewProjection viewProjection_;
	//デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	//自キャラ
	std::unique_ptr<Model> modelFighterBody_ = nullptr;
	std::unique_ptr<Model> modelFighterL_arm_ = nullptr;
	std::unique_ptr<Model> modelFighterR_arm_ = nullptr;
	std::unique_ptr<Model> modelWeapon_ = nullptr;
	std::vector<Model*> playerModels_{};
	uint32_t textureHandlePlayer_ = 0;
	uint32_t textureHandlePlayerDamage_ = 0;
	std::vector<uint32_t*> textureHandlesPlayer_{};
	std::unique_ptr<Player> player_ = nullptr;
	uint32_t soundDataHandleShot_ = 0;
	uint32_t soundDataHandleDamage_ = 0;

	//自機弾
	std::unique_ptr<Model> modelPlayerBullet_ = nullptr;
	std::list<std::unique_ptr<PlayerBullet>> playerBullets_{};

	//敵キャラ
	std::unique_ptr<Model> modelEnemyHead_ = nullptr;
	std::unique_ptr<Model> modelEnemyBody_ = nullptr;
	std::unique_ptr<Model> modelEnemyL_arm_ = nullptr;
	std::unique_ptr<Model> modelEnemyR_arm_ = nullptr;
	std::vector<Model*> enemyModels_{};
	uint32_t textureHandleLife_ = 0;
	uint32_t textureHandleEnemyLv1_ = 0;
	uint32_t textureHandleEnemyLv2_ = 0;
	uint32_t textureHandleEnemyLv3_ = 0;
	std::vector<uint32_t*> textureHandlesEnemy_{};
	std::list<std::unique_ptr<Enemy>> enemys_{};
	uint32_t soundDataHandleExplosion_ = 0;
	int32_t enemySpornTimer_ = 0;
	int32_t enemyLife_ = 3;

	//アイテム
	std::unique_ptr<Model> modelItem_ = nullptr;
	uint32_t textureHandleItem_ = 0;
	std::list<std::unique_ptr<Item>> items_{};
	int32_t itemSpornTimer_ = 0;
	uint32_t soundDataHandleItemGet_ = 0;

	//天球
	std::unique_ptr<Model> modelSkydome_ = nullptr;
	std::unique_ptr<Skydome> skydome_ = nullptr;

	//地面
	std::unique_ptr<Model> modelGround_ = nullptr;
	std::unique_ptr<Ground> ground_ = nullptr;

	//壁
	std::unique_ptr<Model> modelWall_ = nullptr;
	std::array<std::unique_ptr<Wall>, 4> walls_{};

	//追従カメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;

	//パーティクルエミッター
	std::unique_ptr<Model> modelHitParticle_ = nullptr;
	std::unique_ptr<Model> modelDestructionParticle_ = nullptr;
	std::list<std::unique_ptr<ParticleEmitter>> particleEmitters_{};
	uint32_t textureHandleDestructionParticle_[3]{};

	//衝突マネージャー
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;
	
	//スコア
	std::unique_ptr<Score> score_ = nullptr;
	uint32_t textureHandleScore_ = 0;
	uint32_t textureHandleNum_[10]{};
	std::vector<uint32_t*> textureHandlesScore_{};

	//制限時間
	std::unique_ptr<TimeLimit> timeLimit_ = nullptr;
	uint32_t textureHandleColon_ = 0;
	std::vector<uint32_t*> textureHandlesTime_{};

	//BGM
	bool isPlayAudio = false;
	uint32_t soundDataHandleGameScene_ = 0;
	uint32_t voiceHandleGameScene_ = 0;

	//ゲーム終了
	bool isEnd_ = false;
	float blackOutSpeed_ = 0.01f;
	uint32_t textureHandleBlackOut_ = 0;
	std::unique_ptr<Sprite> spriteBlackOut_ = nullptr;

	//HUD
	uint32_t textureHandleGuide_[3]{};
	std::array<std::unique_ptr<Sprite>, 3> spriteHUD_{};

	// シーン切り替えのフラグ
	bool isChange_ = false;
	int32_t nextScene_ = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
