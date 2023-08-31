#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include <cassert>

int32_t GameScene::enemySpornTime;
int32_t GameScene::itemSpornTime;

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバッグカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//自キャラのモデル生成
	modelFighterBody_.reset(Model::CreateFromOBJ("Player_Body", true));
	modelFighterL_arm_.reset(Model::CreateFromOBJ("Player_L_arm", true));
	modelFighterR_arm_.reset(Model::CreateFromOBJ("Player_R_arm", true));
	modelWeapon_.reset(Model::CreateFromOBJ("Weapon", true));
	modelPlayerBullet_.reset(Model::CreateFromOBJ("PlayerBullet",true));
	playerModels_ = {
	    modelFighterBody_.get(), modelFighterL_arm_.get(), modelFighterR_arm_.get(),
	    /*modelWeapon_.get(), */modelPlayerBullet_.get()};
	//自キャラのテクスチャの読み込み
	textureHandlePlayer_ = TextureManager::Load("Player_Body/Player.png");
	textureHandlePlayerDamage_ = TextureManager::Load("Player_Body/PlayerDamage.png");
	textureHandleLife_ = TextureManager::Load("HUD/Life.png");
	textureHandlesPlayer_ = {
	    &textureHandlePlayer_, &textureHandlePlayerDamage_, &textureHandleLife_};
	//射撃音の読み込み
	soundDataHandleShot_ = audio_->LoadWave("Audio/Shot.wav");
	//ダメージ音の読み込み
	soundDataHandleDamage_ = audio_->LoadWave("Audio/Damage.wav");
	//自キャラの生成
	player_ = std::make_unique<Player>();
	//自キャラの初期化
	player_->Initialize(playerModels_, textureHandlesPlayer_, 1);
	//自キャラにゲームシーンをセット
	player_->SetGameScene(this);
	//自キャラに射撃音のサウンドハンドルをセット
	player_->SetSoundDataHandleShot(soundDataHandleShot_);
	// 自キャラにダメージ音のサウンドハンドルをセット
	player_->SetSoundDataHandleDamage(soundDataHandleDamage_);

	//敵キャラのモデル生成
	modelEnemyHead_.reset(Model::CreateFromOBJ("Enemy_Head", true));
	modelEnemyBody_.reset(Model::CreateFromOBJ("Enemy_Body", true));
	modelEnemyL_arm_.reset(Model::CreateFromOBJ("Enemy_L_arm", true));
	modelEnemyR_arm_.reset(Model::CreateFromOBJ("Enemy_R_arm", true));
	enemyModels_ = {
	    modelEnemyHead_.get(), modelEnemyBody_.get(), modelEnemyL_arm_.get(),
	    modelEnemyR_arm_.get()};
	//敵キャラのテクスチャ読み込み
	textureHandleEnemyLv1_ = TextureManager::Load("Enemy_Head/Enemy1.png");
	textureHandleEnemyLv2_ = TextureManager::Load("Enemy_Head/Enemy2.png");
	textureHandleEnemyLv3_ = TextureManager::Load("Enemy_Head/Enemy3.png");
	textureHandlesEnemy_ = {
	    &textureHandleEnemyLv1_, &textureHandleEnemyLv2_, &textureHandleEnemyLv3_};
	//爆発音の読み込み
	soundDataHandleExplosion_ = audio_->LoadWave("Audio/Explosion.wav");
	//ランド関数の初期化
	std::srand(int(time(nullptr)));
	//敵のリスポーンタイマーの初期化
	enemySpornTime = 60 * 2;
	enemySpornTimer_ = enemySpornTime;

	//アイテムのモデル生成
	modelItem_.reset(Model::Create());
	//アイテムのテクスチャを読み込む
	textureHandleItem_ = TextureManager::Load("Item/Item.png");
	//アイテムのリスポーンタイマーの初期化
	itemSpornTime = 60 * 10;
	itemSpornTimer_ = itemSpornTime;
	//アイテム取得音
	soundDataHandleItemGet_ = audio_->LoadWave("Audio/Item.wav");

	//天球のモデル生成
	modelSkydome_.reset(Model::CreateFromOBJ("Skydome", true));
	//天球の生成
	skydome_ = std::make_unique<Skydome>();
	//天球の初期化
	skydome_->Initialize(modelSkydome_.get());

	//地面のモデル生成
	modelGround_.reset(Model::CreateFromOBJ("Ground", true));
	//地面の生成
	ground_ = std::make_unique<Ground>();
	//地面の初期化
	ground_->Initialize(modelGround_.get());

	//壁のモデル生成
	modelWall_.reset(Model::CreateFromOBJ("Wall", true));
	//壁の生成
	for (int i = 0; i < walls_.size(); i++) {
		walls_[i] = std::make_unique<Wall>();
	}
	//壁の初期化
	walls_[0]->Initialize(modelWall_.get(), {-100.0f, 0.0f, 0.0f}, {0.0f, -1.57079632679f, 0.0f});
	walls_[1]->Initialize(modelWall_.get(), {0.0f, 0.0f, 100.0f}, {0.0f, 0.0f, 0.0f});
	walls_[2]->Initialize(modelWall_.get(), {100.0f, 0.0f, 0.0f}, {0.0f, 1.57079632679f, 0.0f});
	walls_[3]->Initialize(modelWall_.get(), {0.0f, 0.0f, -100.0f}, {0.0f, 3.14159265359f, 0.0f});

	//パーティクルのモデル作成
	modelHitParticle_.reset(Model::CreateFromOBJ("HitParticle", true));
	modelDestructionParticle_.reset(Model::Create());
	textureHandleDestructionParticle_[0] =
	    TextureManager::Load("DestructionParticle/DestructionParticleColor1.png");
	textureHandleDestructionParticle_[1] =
	    TextureManager::Load("DestructionParticle/DestructionParticleColor2.png");
	textureHandleDestructionParticle_[2] =
	    TextureManager::Load("DestructionParticle/DestructionParticleColor3.png");

	//追従カメラの作成
	followCamera_ = std::make_unique<FollowCamera>();
	//追従カメラの初期化
	followCamera_->Initialize();
	//自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());
	//追従カメラのビュープロジェクションを自キャラにセット
	player_->SetViewProjection(&followCamera_->GetViewProjection());

	//衝突マネージャーの生成
	collisionManager_ = std::make_unique<CollisionManager>();

	//スコアのテクスチャ読み込み
	textureHandleScore_ = TextureManager::Load("Font/Score.png");
	textureHandleNum_[0] = TextureManager::Load("Font/0.png");
	textureHandleNum_[1] = TextureManager::Load("Font/1.png");
	textureHandleNum_[2] = TextureManager::Load("Font/2.png");
	textureHandleNum_[3] = TextureManager::Load("Font/3.png");
	textureHandleNum_[4] = TextureManager::Load("Font/4.png");
	textureHandleNum_[5] = TextureManager::Load("Font/5.png");
	textureHandleNum_[6] = TextureManager::Load("Font/6.png");
	textureHandleNum_[7] = TextureManager::Load("Font/7.png");
	textureHandleNum_[8] = TextureManager::Load("Font/8.png");
	textureHandleNum_[9] = TextureManager::Load("Font/9.png");
	textureHandlesScore_ = {&textureHandleScore_,  &textureHandleNum_[0], &textureHandleNum_[1],
	                   &textureHandleNum_[2], &textureHandleNum_[3], &textureHandleNum_[4],
	                   &textureHandleNum_[5], &textureHandleNum_[6], &textureHandleNum_[7],
	                   &textureHandleNum_[8], &textureHandleNum_[9]};
	//スコアの生成
	score_ = std::make_unique<Score>();
	//スコアの初期化
	score_->Initialize(textureHandlesScore_);
	//スコアのリセット
	score_->ScoreReset();

	//タイムリミットのテクスチャ読み込み
	textureHandleColon_ = TextureManager::Load("Font/Colon.png");
	textureHandlesTime_ = {&textureHandleColon_,  &textureHandleNum_[0], &textureHandleNum_[1],
	                       &textureHandleNum_[2], &textureHandleNum_[3], &textureHandleNum_[4],
	                       &textureHandleNum_[5], &textureHandleNum_[6], &textureHandleNum_[7],
	                       &textureHandleNum_[8], &textureHandleNum_[9]};
	//タイムリミットの初期化
	timeLimit_ = std::make_unique<TimeLimit>();
	//タイムリミットの初期化
	timeLimit_->Initialize(textureHandlesTime_);

	//BGM
	soundDataHandleGameScene_ = audio_->LoadWave("Audio/GameScene.wav");

	//ブラックアウト用のテクスチャを読み込む
	textureHandleBlackOut_ = TextureManager::Load("BlackOut.png");
	blackOutSpeed_ = 0.01f;
	spriteBlackOut_.reset(
	    Sprite::Create(textureHandleBlackOut_, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}));

	//HUDのテクスチャを読み込む
	textureHandleGuide_[0] = TextureManager::Load("HUD/Attack.png");
	textureHandleGuide_[1] = TextureManager::Load("HUD/Move.png");
	textureHandleGuide_[2] = TextureManager::Load("HUD/Camera.png");
	//HUDのスプライトの作成
	for (int i = 0; i < spriteHUD_.size(); i++){
		spriteHUD_[i].reset(Sprite::Create(textureHandleGuide_[i], {50.0f, i * 30.0f + 580.0f}));
		spriteHUD_[i]->SetSize({spriteHUD_[i]->GetSize().x / 2, spriteHUD_[i]->GetSize().y / 2});
	}
}

void GameScene::Update() {
	//BGM
	if (isPlayAudio == false) {
		isPlayAudio = true; 
		voiceHandleGameScene_ = audio_->PlayWave(soundDataHandleGameScene_, true, 0.3f);
	}

	// デスフラグの立ったパーティクルエミッターを削除
	particleEmitters_.remove_if([](std::unique_ptr<ParticleEmitter>& particleEmitter) {
		if (particleEmitter->isDead()) {
			particleEmitter.reset();
			return true;
		}
		return false;
	});
	// パーティクルの更新
	for (std::unique_ptr<ParticleEmitter>& particleEmitter : particleEmitters_) {
		particleEmitter->Update();
	}

	if (isEnd_) {
		blackOutSpeed_ += 0.01f;
		spriteBlackOut_->SetColor({0.0f, 0.0f, 0.0f, blackOutSpeed_});
		if (blackOutSpeed_ >= 1.0f) {
			isChange_ = true;
			nextScene_ = 2;
		}
		return;
	}

	// 自キャラの更新
	player_->Update();

	//デスフラグの立った自弾を削除
	playerBullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		if (bullet->isDead()) {
			bullet.reset();
			return true;
		}
		return false;
	});
	// 自弾の更新
	for (std::unique_ptr<PlayerBullet>& bullet : playerBullets_) {
		bullet->Update();
	}

	//デスフラグの立った敵を削除
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		if (enemy->isDead()) {
			enemy.reset();
			return true;
		}
		return false;
	});
	//敵のスポーン処理
	GameScene::EnemySporn();
	//敵キャラの更新
	for (std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Update();
	}

	//デスフラグの立ったアイテムを削除
	items_.remove_if([](std::unique_ptr<Item>& item) {
		if (item->isDead()) {
			item.reset();
			return true;
		}
		return false;
	});
	//アイテムのスポーン処理
	GameScene::ItemSporn();
	//アイテムの更新
	for (std::unique_ptr<Item>& item : items_) {
		item->Update();
	}

	//天球の更新
	skydome_->Update();

	//地面の更新
	ground_->Update();

	//壁の更新
	for (int i = 0; i < walls_.size(); i++) {
		walls_[i]->Update();
	}

	//追従カメラの更新
	followCamera_->Update();

	//衝突マネージャーのリストをクリア
	collisionManager_->ClearColliderList();
	//自キャラを衝突マネージャーのリストに追加
	collisionManager_->SetColliderList(player_.get());
	//自弾を衝突マネージャーのリストに追加
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets_) {
		collisionManager_->SetColliderList(bullet.get());
	}
	//敵を衝突マネージャーのリストに追加
	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		collisionManager_->SetColliderList(enemy.get());
	}
	//衝突判定
	collisionManager_->CheckAllCollisions();

	//アイテムとプレイヤーの当たり判定
	Vector3 playerPos = player_->GetWorldPosition();
	for (std::unique_ptr<Item>& item : items_) {
		Vector3 itemPos = item->GetWorldPosition();
		float distance = Length(Subtract(playerPos, itemPos));
		if (distance <= player_->GetRadius() + item->GetRadius()) {
			//アイテムを消す
			item->OnCollision();
			//強化状態にする
			player_->SetStrongState();
			//アイテム取得音を鳴らす
			audio_->PlayWave(soundDataHandleItemGet_, false, 1.0f);
		}
	}

	//スコアの更新
	score_->Update();

	//制限時間の更新
	timeLimit_->Update();

	// 追従カメラのビュー行列をゲームシーンのビュープロジェクションにコピー
	viewProjection_.matView = followCamera_->GetViewProjection().matView;
	// 追従カメラのプロジェクション行列をゲームシーンのビュープロジェクションにコピー
	viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;

#ifdef _DEBUG
	//デバッグカメラの切り替え
	if (input_->TriggerKey(DIK_1)) {
		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		} else {
			isDebugCameraActive_ = true;
		}
	}
#endif
	//デバッグカメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}

	// ゲーム終了処理
	//自キャラのライフが0になった時
	if (player_->isDead()) {
		//gameManager->ChangeScene(new GameResult());
		isEnd_ = true;
	}
	//制限時間内生き残れた時
	else if (timeLimit_->GetElapsedTime() == 180) {
		//gameManager->ChangeScene(new GameResult());
		isEnd_ = true;
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	//自キャラの描画
	player_->Draw(viewProjection_);

	//自弾の描画
	for (std::unique_ptr<PlayerBullet>& bullet : playerBullets_) {
		bullet->Draw(viewProjection_);
	}

	//敵キャラの描画
	for (std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}

	//アイテムの描画
	for (std::unique_ptr<Item>& item : items_) {
		item->Draw(viewProjection_);
	}

	//天球の描画
	skydome_->Draw(viewProjection_);

	//地面の描画
	ground_->Draw(viewProjection_);

	//壁の描画
	for (int i = 0; i < walls_.size(); i++) {
		walls_[i]->Draw(viewProjection_);
	}

	//パーティクルの描画
	for (std::unique_ptr<ParticleEmitter>& particleEmitter : particleEmitters_) {
		particleEmitter->Draw(viewProjection_);
	}

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	//プレイヤーのUI描画
	player_->DrawUI();

	//スコアの描画
	score_->Draw();

	//タイムリミットの描画
	timeLimit_->Draw();

	//HUDの描画
	for (int i = 0; i < spriteHUD_.size(); i++) {
		spriteHUD_[i]->Draw();
	}

	//ブラックアウト
	spriteBlackOut_->Draw();

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::EnemySporn() {
	//敵のスポーンタイマーが0以下になったら敵をスポーンさせる
	if (--enemySpornTimer_ <= 0) {
		//タイマーを戻す
		enemySpornTimer_ = enemySpornTime;
		//敵キャラの生成
		Enemy* enemy = new Enemy();
		//0~30秒
		if (timeLimit_->GetElapsedTime() <= 30) {
			enemy->Initialize(enemyModels_, textureHandlesEnemy_, 1);
		}
		//31~60秒
		else if (timeLimit_->GetElapsedTime() > 30 && timeLimit_->GetElapsedTime() <= 60) {
			enemySpornTime = 100;
			enemy->Initialize(enemyModels_, textureHandlesEnemy_, rand() % 2 + 1);
		} 
		//61~90秒
		else if (timeLimit_->GetElapsedTime() > 60 && timeLimit_->GetElapsedTime() <= 90) {
			enemy->Initialize(enemyModels_, textureHandlesEnemy_, 2);
		} 
		//91~120秒
		else if (timeLimit_->GetElapsedTime() > 90 && timeLimit_->GetElapsedTime() <= 120) {
			enemy->Initialize(enemyModels_, textureHandlesEnemy_, rand() % 2 + 2);
		}
		//121~150秒
		else if (timeLimit_->GetElapsedTime() > 120 && timeLimit_->GetElapsedTime() <= 150) {
			enemySpornTime = 80;
			enemy->Initialize(enemyModels_, textureHandlesEnemy_, rand() % 2 + 2);
		} 
		//151~180秒
		else if (timeLimit_->GetElapsedTime() > 150) {
			enemy->Initialize(enemyModels_, textureHandlesEnemy_, 3);
		} 
		//敵キャラにプレイヤーをセット
		enemy->SetPlayer(player_.get());
		//敵キャラにゲームシーンをセット
		enemy->SetGameScene(this);
		//敵キャラにスコアをセット
		enemy->SetScore(score_.get());
		//敵キャラに爆発音のサウンドハンドルをセット
		enemy->SetSoundDataHandle(soundDataHandleExplosion_);
		//敵キャラのリストに追加する
		enemys_.push_back(std::unique_ptr<Enemy>(enemy));
	}
}

void GameScene::ItemSporn() {
	if (--itemSpornTimer_ <= 0) {
		itemSpornTimer_ = itemSpornTime;
		// アイテムの作成
		Item* item = new Item();
		// アイテムの初期化
		item->Initialize(modelItem_.get(), textureHandleItem_);
		items_.push_back(std::unique_ptr<Item>(item));
	}
}

void GameScene::AddPlayerBullet(PlayerBullet* playerBullet) {
	//自機の弾を追加する
	playerBullets_.push_back(std::unique_ptr<PlayerBullet>(playerBullet));
}

void GameScene::CreateParticleEmitter(
    ParticleType particleType, const Vector3& position, int32_t enemyLevel){
	//パーティクルを作成
	ParticleEmitter* particleEmitter = new ParticleEmitter();
	//ヒットパーティクルの初期化
	if (particleType == ParticleType::Hit) {
		particleEmitter->Initialize(modelHitParticle_.get(), 0, position, particleType);
	} 
	//破壊パーティクルの初期化
	else if (particleType == ParticleType::Destruction) {
		if (enemyLevel == 1) {
			particleEmitter->Initialize(
			    modelDestructionParticle_.get(), textureHandleDestructionParticle_[0], position,
			    particleType);
		} else if (enemyLevel == 2) {
			particleEmitter->Initialize(
			    modelDestructionParticle_.get(), textureHandleDestructionParticle_[1], position,
			    particleType);
		} else if (enemyLevel == 3) {
			particleEmitter->Initialize(
			    modelDestructionParticle_.get(), textureHandleDestructionParticle_[2], position,
			    particleType);
		}
	}
	//パーティクルを追加する
	particleEmitters_.push_back(std::unique_ptr<ParticleEmitter>(particleEmitter));
}


void GameScene::Reset() {
	//切り替えフラグをfalseにする
	isChange_ = false;
	//BGMを止める
	audio_->StopWave(voiceHandleGameScene_);
	//フラグをfalseのする
    isPlayAudio = false;
	//終了フラグをfalseにする
	isEnd_ = false;
	//ブラックアウトの初期化
	blackOutSpeed_ = 0.0f;
	spriteBlackOut_->SetColor({0.0f, 0.0f, 0.0f, blackOutSpeed_});
	//自キャラのリセット
	player_->Reset();
	//追尾カメラのリセット
	followCamera_->Reset();
	//タイマーのリセット
	timeLimit_->Reset();

	// 自弾のリセット
	for (std::unique_ptr<PlayerBullet>& bullet : playerBullets_) {
		bullet.reset();
	}
	playerBullets_.clear();

	// 敵キャラの描画
	for (std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy.reset();
	}
	enemys_.clear();

	// アイテムの描画
	for (std::unique_ptr<Item>& item : items_) {
		item.reset();
	}
	items_.clear();
}
