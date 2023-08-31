#include "TimeLimit.h"
#include "ImGuiManager.h"

void TimeLimit::Initialize(const std::vector<uint32_t*>& textureHandles) {
	//テクスチャハンドルを取得
	textureHandles_ = textureHandles;
	//制限時間のスプレイト生成
	spriteColon_.reset(
	    Sprite::Create(*textureHandles_[0], {625.0f, 27.5f}, {1.0f, 1.0f, 0.0f, 1.0f}));
	for (int i = 0; i < 2; i++) {
		spriteNum_[i].reset(Sprite::Create(
		    *textureHandles_[1], {i * 35.0f + 550.0f, 30.0f}, {1.0f, 1.0f, 0.0f, 1.0f}));
	}
	for (int i = 2; i < 4; i++) {
		spriteNum_[i].reset(Sprite::Create(
		    *textureHandles_[1], {i * 35.0f + 590.0f, 30.0f}, {1.0f, 1.0f, 0.0f, 1.0f}));
	}
	//ゲーム開始時間の初期化
	startTime_ = time(nullptr);
}

void TimeLimit::Update() { 
	// 前のフレームの時間を取得
	oldTime_ = elapsedTime_;
	if (startFlag_ == false) {
		startFlag_ = true;
		startTime_ = time(nullptr);
	}
	//現在の時間を取得
	currentTime_ = time(nullptr); 
	//現在の時間からゲーム開始時間を引いて経過時間を取得
	elapsedTime_ = currentTime_ - startTime_;
	//制限時間から引く
	if (oldTime_ != elapsedTime_) {
		timeSeconds_ -= 1;
	}
	
	if (timeSeconds_ <= -1) {
		timeSeconds_ = 59;
		timeMinutes_--;
	}

	//分の位の数字を取得
	int32_t minutesNum = int32_t(timeMinutes_);
	if (minutesNum / 10 >= 0) {
		eachNumber_[0] = minutesNum / 10;
	}
	minutesNum = minutesNum % 10;
	eachNumber_[1] = minutesNum;

	//秒の位の数字を取得
	int32_t secondsNum = int32_t(timeSeconds_);
	if (secondsNum / 10 >= 0) {
		eachNumber_[2] = secondsNum / 10;
	}
	secondsNum = secondsNum % 10;
	eachNumber_[3] = secondsNum;
}

void TimeLimit::Draw() { 
	//コロンの描画
	spriteColon_->Draw();
	//位に応じて数字のスプライトを描画
	for (int i = 0; i < 4; i++) {
		if (eachNumber_[i] == 0) {
			spriteNum_[i]->SetTextureHandle(*textureHandles_[1]);
		} else if (eachNumber_[i] == 1) {
			spriteNum_[i]->SetTextureHandle(*textureHandles_[2]);
		} else if (eachNumber_[i] == 2) {
			spriteNum_[i]->SetTextureHandle(*textureHandles_[3]);
		} else if (eachNumber_[i] == 3) {
			spriteNum_[i]->SetTextureHandle(*textureHandles_[4]);
		} else if (eachNumber_[i] == 4) {
			spriteNum_[i]->SetTextureHandle(*textureHandles_[5]);
		} else if (eachNumber_[i] == 5) {
			spriteNum_[i]->SetTextureHandle(*textureHandles_[6]);
		} else if (eachNumber_[i] == 6) {
			spriteNum_[i]->SetTextureHandle(*textureHandles_[7]);
		} else if (eachNumber_[i] == 7) {
			spriteNum_[i]->SetTextureHandle(*textureHandles_[8]);
		} else if (eachNumber_[i] == 8) {
			spriteNum_[i]->SetTextureHandle(*textureHandles_[9]);
		} else if (eachNumber_[i] == 9) {
			spriteNum_[i]->SetTextureHandle(*textureHandles_[10]);
		}
		spriteNum_[i]->Draw();
	}
}

void TimeLimit::Reset() { 
	startFlag_ = false;
	timeMinutes_ = kTimeLimit;
	timeSeconds_ = 0;
}