#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <ImGuiManager.h>
#include "AxisIndicator.h"
#include "math/MathFunction.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete debugCamera_;
	delete enemy_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	//ファイルを指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("sample.png");
	//3Dモデルの生成
	model_ = Model::Create();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	//敵の生成
	enemy_ = new Enemy();
	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);
	//敵の初期化
	enemy_->Initialize(model_, textureHandle_);
}

void GameScene::Update() {
	player_->Update(); 
	enemy_->Update();
	CheckAllCollisions();
	//カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
	    //ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}

	#ifdef _DEBUG
	if (isDebugCameraActive_ == false) {
		if (input_->TriggerKey(DIK_1)) {
			isDebugCameraActive_ = true;
		}
	}
	else{
		if (input_->TriggerKey(DIK_1)) {
			isDebugCameraActive_ = false;
		}
	}
	#endif
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

void GameScene::CheckAllCollisions() {
	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetEnemyBullets();

	#pragma region 自キャラと敵弾の当たり判定
	//自キャラと敵弾すべての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		//ペアの衝突判定
		CheckCollisionPair(player_, bullet);
	}
	#pragma endregion

	#pragma region 自弾と敵キャラの当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		// ペアの衝突判定
		CheckCollisionPair(enemy_, bullet);
	}
	#pragma endregion

	#pragma region 自弾と敵弾の当たり判定
	for (PlayerBullet* playerBullet : playerBullets) {
		for (EnemyBullet* enemyBullet : enemyBullets) {
			// ペアの衝突判定
			CheckCollisionPair(playerBullet, enemyBullet);
		}
	}
	#pragma endregion
}

void GameScene::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	Vector3 posA = colliderA->GetWorldPosition();
	Vector3 posB = colliderB->GetWorldPosition();
	float distance = Length(Subtract(posA, posB));
	//球と球の交差判定
	if (distance <= colliderA->GetRadius() + colliderB->GetRadius()) {
		//コライダーAの衝突時コールバックを呼び出す
		colliderA->OnCollision();
		//コライダーBの衝突時コールバックを呼び出す
		colliderB->OnCollision();
	}
}