#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include "math/MathFunction.h"
#include <ImGuiManager.h>
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete debugCamera_;
	for (Enemy* enemy : enemys_) {
		delete enemy;
	}
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
	textureReticle_ = TextureManager::Load("reticle.png");
	lockonTexture_ = TextureManager::Load("lockonReticle.png");
	// 3Dモデルの生成
	model_ = Model::Create();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	// RailCamera
	railCamera_ = new RailCamera();
	railCamera_->Initialize({0, 0, -30}, {0, 0, 0});
	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	Vector3 playerPositon = {0.0f, 0.0f, 30.0f};
	player_->Initialize(model_, textureHandle_, playerPositon, textureReticle_, lockonTexture_);
	// 自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	// 敵の生成
	Enemy* enemy = new Enemy();
	// 敵キャラに自キャラのアドレスを渡す
	enemy->SetPlayer(player_);
	// 敵キャラにゲームシーンを渡す
	enemy->SetGameScene(this);
	// 敵の初期化
	Vector3 enemyPosition = {10.0f, 0.0f, 50.0f};
	enemy->Initialize(model_, textureHandle_, enemyPosition);
	//リストに登録する
	enemys_.push_back(enemy);
	//敵発生データ読み込み
	GameScene::LoadEnemyPopData();
	// 衝突マネージャーの生成
	collisionManager_ = new CollisionManager();
	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// skydome
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelSkydome_);
	//自キャラに敵キャラを渡す
	player_->SetEnemy(enemys_);
}

void GameScene::Update() {
	// 敵生成
	GameScene::UpdateEnemyPopCommands();
	//デスフラグが立った敵を削除
	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->isDead()) {
			delete enemy;
			return true;
		}
		return false;
	});
	//敵更新
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}
	// デスフラグの立った弾を削除
	enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		if (bullet->isDead()) {
			bullet.reset();
			return true;
		}
		return false;
	});
	//敵弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Update();
	}
	// 自キャラに敵キャラを渡す
	player_->SetEnemy(enemys_);
	// 自機更新
	player_->Update(viewProjection_);
	//天球更新
	skydome_->Update();
	// CollisionManagerのリストをクリア
	collisionManager_->ClearColliderList();
	// CollisionManagerのリストを追加
	collisionManager_->SetColliderList(player_);
	for (Enemy* enemy : enemys_) {
		collisionManager_->SetColliderList(enemy);
	}
	// 自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	// 自弾すべてについて
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		collisionManager_->SetColliderList(bullet.get());
	}
	// 敵弾すべてについて
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
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
	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}
	//敵弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}
	skydome_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	player_->DrawUI();

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	//リストに登録する
	enemyBullets_.push_back(std::unique_ptr<EnemyBullet>(enemyBullet));
}

void GameScene::AddEnemy(const Vector3& pos) {
	Enemy* enemy = new Enemy();
	// 敵キャラに自キャラのアドレスを渡す
	enemy->SetPlayer(player_);
	// 敵キャラにゲームシーンを渡す
	enemy->SetGameScene(this);
	// 敵の初期化
	enemy->Initialize(model_, textureHandle_, pos);
	// リストに登録する
	enemys_.push_back(enemy);
}

void GameScene::LoadEnemyPopData() {
	//ファイルを開く
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	//待機処理
	if (waitingFlag) {
		waitTimer--;
		if (waitTimer <= 0) {
			//待機完了
			waitingFlag = false;
		}
		return;
	}

	//一桁分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		//一桁分の文字列をストリームに変換して解析しやすくなる
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先端文字列を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}

		//POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//敵を発生させる
			GameScene::AddEnemy(Vector3(x, y, z));
		}

		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			waitingFlag = true;
			waitTimer = waitTime;

			//コマンドループを抜ける
			break;
		}
	}
}