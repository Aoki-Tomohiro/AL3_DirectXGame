#include "GameResult.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include <cassert>

GameResult::GameResult() {}

GameResult::~GameResult() {}

void GameResult::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//スコアのテクスチャ読み込み
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
	textureHandleScore_ = TextureManager::Load("Font/Score.png");
	textureHandleKills_ = TextureManager::Load("Font/Kills.png");
	textureHandleLife_ = TextureManager::Load("Font/Life.png");

	//キル数のスプライト作成
	spriteKills_.reset(Sprite::Create(textureHandleKills_, {285.0f, 430.0f}));
	for (int i = 0; i < spriteKillsNum_.size(); i++) {
		spriteKillsNum_[i].reset(Sprite::Create(
		    textureHandleNum_[0], {i * 35 + 500.0f, 430.0f}));
	}

	//残りライフのスプライト作成
	spriteLife_.reset(Sprite::Create(textureHandleLife_, {675.0f, 430.0f}));
	spriteLifeNum_.reset(
	    Sprite::Create(textureHandleNum_[0], {640.0f + 225.0f, 430.0f}));

	//スコアのスプライト生成
	spriteScore_.reset(
	    Sprite::Create(textureHandleScore_, {220, 180.0f}, {1.0f, 1.0f, 0.0f, 1.0f}));
	spriteScore_->SetSize({500.0f, 100.0f});
	for (int i = 0; i < 5; i++) {
		spriteNumScore_[i].reset(
		    Sprite::Create(textureHandleNum_[0], {i * 70.0f + 700.0f, 180.0f}, {1.0f,1.0f,0.0f,1.0f}));
		spriteNumScore_[i]->SetSize({100.0f, 100.0f});
	}

	// ブラックアウト用のテクスチャを読み込む
	textureHandleResult_ = TextureManager::Load("Result/Result.png");
	spriteResult_.reset(Sprite::Create(textureHandleResult_, {0.0f, 0.0f}));

	//スコアの生成
	score_ = std::make_unique<Score>();
	//プレイヤーの生成
	player_ = std::make_unique<Player>();
}

void GameResult::Update() {
	//ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	//ゲームパッド未接続なら何もせず抜ける
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			// A ボタンが押された
			isChange_ = true;
			nextScene_ = 1;
		} else if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			// B ボタンが押された
			isChange_ = true;
			nextScene_ = 0;
		}
	}

	//スコア
	int32_t scoreNum = score_->GetScore();
	eachNumberScore_[0] = scoreNum / 10000;
	scoreNum = scoreNum % 10000;
	eachNumberScore_[1] = scoreNum / 1000;
	scoreNum = scoreNum % 1000;
	eachNumberScore_[2] = scoreNum / 100;
	scoreNum = scoreNum % 100;
	eachNumberScore_[3] = scoreNum / 10;
	scoreNum = scoreNum % 10;
	eachNumberScore_[4] = scoreNum;

	//キル数
	int32_t killCount = score_->GetKillCount();
	eachNumberKills_[0] = killCount / 100;
	killCount = killCount % 100;
	eachNumberKills_[1] = killCount / 10;
	killCount = killCount % 10;
	eachNumberKills_[2] = killCount;
}

void GameResult::Draw() {

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

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	//黒背景
	spriteResult_->Draw();

	//スコアの描画
	spriteScore_->Draw();
	// 位に応じて数字のスプライトを描画
	for (int i = 0; i < 5; i++) {
		if (eachNumberScore_[i] == 0) {
			spriteNumScore_[i]->SetTextureHandle(textureHandleNum_[0]);
		} else if (eachNumberScore_[i] == 1) {
			spriteNumScore_[i]->SetTextureHandle(textureHandleNum_[1]);
		} else if (eachNumberScore_[i] == 2) {
			spriteNumScore_[i]->SetTextureHandle(textureHandleNum_[2]);
		} else if (eachNumberScore_[i] == 3) {
			spriteNumScore_[i]->SetTextureHandle(textureHandleNum_[3]);
		} else if (eachNumberScore_[i] == 4) {
			spriteNumScore_[i]->SetTextureHandle(textureHandleNum_[4]);
		} else if (eachNumberScore_[i] == 5) {
			spriteNumScore_[i]->SetTextureHandle(textureHandleNum_[5]);
		} else if (eachNumberScore_[i] == 6) {
			spriteNumScore_[i]->SetTextureHandle(textureHandleNum_[6]);
		} else if (eachNumberScore_[i] == 7) {
			spriteNumScore_[i]->SetTextureHandle(textureHandleNum_[7]);
		} else if (eachNumberScore_[i] == 8) {
			spriteNumScore_[i]->SetTextureHandle(textureHandleNum_[8]);
		} else if (eachNumberScore_[i] == 9) {
			spriteNumScore_[i]->SetTextureHandle(textureHandleNum_[9]);
		}
		spriteNumScore_[i]->Draw();
	}

	//キル数の描画
	spriteKills_->Draw();
	for (int i = 0; i < spriteKillsNum_.size(); i++) {
		if (eachNumberKills_[i] == 0) {
			spriteKillsNum_[i]->SetTextureHandle(textureHandleNum_[0]);
		} else if (eachNumberKills_[i] == 1) {
			spriteKillsNum_[i]->SetTextureHandle(textureHandleNum_[1]);
		} else if (eachNumberKills_[i] == 2) {
			spriteKillsNum_[i]->SetTextureHandle(textureHandleNum_[2]);
		} else if (eachNumberKills_[i] == 3) {
			spriteKillsNum_[i]->SetTextureHandle(textureHandleNum_[3]);
		} else if (eachNumberKills_[i] == 4) {
			spriteKillsNum_[i]->SetTextureHandle(textureHandleNum_[4]);
		} else if (eachNumberKills_[i] == 5) {
			spriteKillsNum_[i]->SetTextureHandle(textureHandleNum_[5]);
		} else if (eachNumberKills_[i] == 6) {
			spriteKillsNum_[i]->SetTextureHandle(textureHandleNum_[6]);
		} else if (eachNumberKills_[i] == 7) {
			spriteKillsNum_[i]->SetTextureHandle(textureHandleNum_[7]);
		} else if (eachNumberKills_[i] == 8) {
			spriteKillsNum_[i]->SetTextureHandle(textureHandleNum_[8]);
		} else if (eachNumberKills_[i] == 9) {
			spriteKillsNum_[i]->SetTextureHandle(textureHandleNum_[9]);
		}
		spriteKillsNum_[i]->Draw();
	}

	//残りライフの描画
	spriteLife_->Draw();

	if (player_->GetLife() == 0) {
		spriteLifeNum_->SetTextureHandle(textureHandleNum_[0]);
		spriteLifeNum_->Draw();
	} else if (player_->GetLife() == 1) {
		spriteLifeNum_->SetTextureHandle(textureHandleNum_[1]);
		spriteLifeNum_->Draw();
	} else if (player_->GetLife() == 2) {
		spriteLifeNum_->SetTextureHandle(textureHandleNum_[2]);
		spriteLifeNum_->Draw();
	} else if (player_->GetLife() == 3) {
		spriteLifeNum_->SetTextureHandle(textureHandleNum_[3]);
		spriteLifeNum_->Draw();
	}

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameResult::Reset() {
	// 切り替えフラグをfalseにする
	isChange_ = false;
	//スコアをリセットする
	score_->ScoreReset();
	//プレイヤーをリセットする
	player_->LifeReset();
}
