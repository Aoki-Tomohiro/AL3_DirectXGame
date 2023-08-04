#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include <cassert>

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
	//自キャラの3Dモデル生成
	modelFighterBody_.reset(Model::CreateFromOBJ("Player_Body", true));
	modelFighterL_arm_.reset(Model::CreateFromOBJ("Player_L_arm", true));
	modelFighterR_arm_.reset(Model::CreateFromOBJ("Player_R_arm", true));
	modelWeapon_.reset(Model::CreateFromOBJ("Weapon", true));
	//自キャラモデルのリスト
	std::vector<Model*> playerModels = {
	    modelFighterBody_.get(), modelFighterL_arm_.get(), modelFighterR_arm_.get(),
	    modelWeapon_.get()};
	//自キャラの生成
	player_ = std::make_unique<Player>();
	//自キャラの初期化
	player_->Initialize(playerModels);
	//敵キャラのモデルのリスト
	std::vector<Model*> enemyModels = {modelFighterBody_.get()};
	//敵キャラの生成
	enemy_ = std::make_unique<Enemy>();
	//敵キャラの初期化
	enemy_->Initialize(enemyModels);
	//天球の3Dモデル生成
	modelSkydome_.reset(Model::CreateFromOBJ("Skydome", true));
	//天球の生成
	skydome_ = std::make_unique<Skydome>();
	//天球の初期化
	skydome_->Initialize(modelSkydome_.get());
	//地面の3Dモデル生成
	modelGround_.reset(Model::CreateFromOBJ("Ground", true));
	//地面の生成
	ground_ = std::make_unique<Ground>();
	//地面の初期化
	ground_->Initialize(modelGround_.get());
	//追従カメラの作成
	followCamera_ = std::make_unique<FollowCamera>();
	//追従カメラの初期化
	followCamera_->Initialize();
	//自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());
	//追従カメラのビュープロジェクションを自キャラにセット
	player_->SetViewProjection(&followCamera_->GetViewProjection());
}

void GameScene::Update() {
	//自キャラの更新
	player_->Update();
	//敵キャラの更新
	enemy_->Update();
	//天球の更新
	skydome_->Update();
	//地面の更新
	ground_->Update();
	//追従カメラの更新
	followCamera_->Update();
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

	//カメラの処理
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

	ImGui::Begin(" ");
	ImGui::Text("1 : DebugCamera");
	ImGui::End();
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
	//敵キャラの描画
	enemy_->Draw(viewProjection_);
	//天球の描画
	skydome_->Draw(viewProjection_);
	//地面の描画
	ground_->Draw(viewProjection_);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

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
