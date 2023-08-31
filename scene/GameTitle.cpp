#include "GameTitle.h"
#include "TextureManager.h"
#include "MathFunction.h"
#include <cassert>

GameTitle::GameTitle() {}

GameTitle::~GameTitle() {}

void GameTitle::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	viewProjection_.rotation_.x = 0.2f;

	//テクスチャ読み込み
	textureHandleTitle_ = TextureManager::Load("Title/Title.png");
	//スプライトの作成
	spriteTitle_.reset(Sprite::Create(textureHandleTitle_, {0.0f, 0.0f}));

	//壁の作成
	modelWall_.reset(Model::CreateFromOBJ("Wall", true));
	// ワールドトランスフォームの初期化
	worldTransformWall_[0].Initialize();
	worldTransformWall_[0].translation_ = {-100.0f, 0.0f, 0.0f};
	worldTransformWall_[0].rotation_ = {0.0f, -1.57079632679f, 0.0f};
	worldTransformWall_[1].Initialize();
	worldTransformWall_[1].translation_ = {0.0f, 0.0f, 100.0f};
	worldTransformWall_[1].rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformWall_[2].Initialize();
	worldTransformWall_[2].translation_ = {100.0f, 0.0f, 0.0f};
	worldTransformWall_[2].rotation_ = {0.0f, 1.57079632679f, 0.0f};
	worldTransformWall_[3].Initialize();
	worldTransformWall_[3].translation_ = {0.0f, 0.0f, -100.0f};
	worldTransformWall_[3].rotation_ = {0.0f, 3.14159265359f, 0.0f};

	//地面のモデル生成
	modelGround_.reset(Model::CreateFromOBJ("Ground", true));
	//地面のワールドトランスフォームの初期化
	worldTransformGround_.Initialize();

	//自キャラのモデル生成
	//自キャラのモデル生成
	modelFighterBody_.reset(Model::CreateFromOBJ("Player_Body", true));
	modelFighterL_arm_.reset(Model::CreateFromOBJ("Player_L_arm", true));
	modelFighterR_arm_.reset(Model::CreateFromOBJ("Player_R_arm", true));
	//自キャラのワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ワールドトランスフォームの初期化
	worldTransformBody_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.translation_.x = 0.8f;
	worldTransformL_arm_.translation_.y = 1.0f;
	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.translation_.x = -0.8f;
	worldTransformR_arm_.translation_.y = 1.0f;
	// 親子関係
	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;

	//追従対象を設定
	target_ = &worldTransform_;

	textureHandleGuide_ = TextureManager::Load("Guide/Guide.png");
	spriteGuide_.reset(Sprite::Create(textureHandleGuide_, {0.0f, 0.0f}));
	guideFlag_ = false;

	soundDataHandleTitle_ = audio_->LoadWave("Audio/Title.wav");
}

void GameTitle::Update() {
	// BGM
	if (isPlayAudio_ == false) {
		isPlayAudio_ = true;
		voiceHandleTitle_ = audio_->PlayWave(soundDataHandleTitle_, true, 0.3f);
	}

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	// ゲームパッド未接続なら何もせず抜ける
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			////Bボタンが押された
			//isChange_ = true;
			//nextScene_ = 1;
			guideFlag_ = true;
		}
	}

	if (guideFlag_ == true) {
		guideTimer_++;
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
				//Bボタンが押された
				if (guideTimer_ > 30) {
					isChange_ = true;
					nextScene_ = 1;
					guideTimer_ = 0;
					guideFlag_ = false;
				}
			}
		}
	}

	//壁のワールドトランスフォームの更新
	for (int i = 0; i < 4; i++) {
		worldTransformWall_[i].UpdateMatrix();
	}

	//地面のワールドトランスフォームの更新
	worldTransformGround_.UpdateMatrix();

	//自キャラのワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

	// 追従対象がいれば
	if (target_) {
		// 追従対象からカメラまでのオフセット(0度の時の値)
		Vector3 offset = {0.0f, 2.0f, -20.0f};

		// カメラの角度から回転行列を計算する
		Matrix4x4 rotateXMatrix = MakeRotateXMatrix(viewProjection_.rotation_.x);
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(viewProjection_.rotation_.y);
		Matrix4x4 rotateZMatrix = MakeRotateZMatrix(viewProjection_.rotation_.z);
		Matrix4x4 rotateMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));
		// オフセットをカメラの回転に合わせて回転させる
		offset = TransformNormal(offset, rotateMatrix);

		// 座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = Add(target_->translation_, offset);
	}

	// ビュー行列の更新
	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();
}

void GameTitle::Draw() {

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

	//壁の描画
	for (int i = 0; i < 4; i++) {
		modelWall_->Draw(worldTransformWall_[i], viewProjection_);
	}

	//地面の描画
	modelGround_->Draw(worldTransformGround_, viewProjection_);

	//自キャラの描画
	modelFighterBody_->Draw(worldTransformBody_, viewProjection_);
	modelFighterL_arm_->Draw(worldTransformL_arm_, viewProjection_);
	modelFighterR_arm_->Draw(worldTransformR_arm_, viewProjection_);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	spriteTitle_->Draw();

	if (guideFlag_ == true) {
		spriteGuide_->Draw();
	}

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameTitle::Reset() { 
	//切り替えフラグをfalseにする
	isChange_ = false;
	guideFlag_ = false;
	//BGMを止める
	audio_->StopWave(voiceHandleTitle_);
	// フラグをfalseのする
	isPlayAudio_ = false;
}
