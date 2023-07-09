#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include "math/MathFunction.h"
#include <ImGuiManager.h>
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete debugCamera_;
	delete enemy_;
	delete collisionManager_;
	delete modelSkydome_;
	skydome_.release();
	delete railCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	// ファイルを指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("sample.png");
	// 3Dモデルの生成
	model_ = Model::Create();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	// RailCamera
	railCamera_ = new RailCamera();
	railCamera_->Initialize({0, 0, -50}, {0, 0, 0});
	// 自キャラの生成
	player_ = new Player();
	// 自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());
	// 自キャラの初期化
	Vector3 playerPositon = {0.0f, 0.0f, 50.0f};
	player_->Initialize(model_, textureHandle_, playerPositon);
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	// 敵の生成
	enemy_ = new Enemy();
	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);
	// 敵の初期化
	enemy_->Initialize(model_, textureHandle_);
	// 衝突マネージャーの生成
	collisionManager_ = new CollisionManager();
	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// skydome
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelSkydome_);
}

void GameScene::Update() {
	player_->Update();
	enemy_->Update();
	skydome_->Update();
	// CollisionManagerのリストをクリア
	collisionManager_->ClearColliderList();
	// CollisionManagerのリストを追加
	collisionManager_->SetColliderList(player_);
	collisionManager_->SetColliderList(enemy_);
	// 自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	// 自弾すべてについて
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		collisionManager_->SetColliderList(bullet.get());
	}
	// 敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetEnemyBullets();
	// 敵弾すべてについて
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		collisionManager_->SetColliderList(bullet.get());
	}
	collisionManager_->CheckAllCollisions();

	// デバッグカメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}

#ifdef _DEBUG
	if (isDebugCameraActive_ == false) {
		if (input_->TriggerKey(DIK_1)) {
			isDebugCameraActive_ = true;
		}
	} else {
		if (input_->TriggerKey(DIK_1)) {
			isDebugCameraActive_ = false;
		}
	}
#endif

	// レールカメラの処理
	if (isDebugCameraActive_ == false) {
		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
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

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}