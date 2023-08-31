#include "Score.h"
#include "TextureManager.h"

int32_t Score::sScore_;
int32_t Score::sKillCount_;

void Score::Initialize(const std::vector<uint32_t*>& textureHandles) {
	//テクスチャハンドルの取得
	textureHandles_ = textureHandles;
	// スコアの生成
	spriteScore_.reset(
	    Sprite::Create(*textureHandles_[0], {865.0f, 30.0f}, {1.0f, 1.0f, 0.0f, 1.0f}));
	for (int i = 0; i < 5; i++) {
		spriteNum_[i].reset(Sprite::Create(
		    *textureHandles_[1], {i * 35.0f + 1075.0f, 30}, {1.0f, 1.0f, 0.0f, 1.0f}));
	}
}

void Score::Update() {
	int32_t scoreNum = sScore_;
	// 10000の位
	eachNumber[0] = scoreNum / 10000;
	scoreNum = scoreNum % 10000;
	// 1000の位
	eachNumber[1] = scoreNum / 1000;
	scoreNum = scoreNum % 1000;
	// 100の位
	eachNumber[2] = scoreNum / 100;
	scoreNum = scoreNum % 100;
	// 10の位
	eachNumber[3] = scoreNum / 10;
	scoreNum = scoreNum % 10;
	// 1の位
	eachNumber[4] = scoreNum;
}

void Score::Draw() {
	// スコアの描画
	spriteScore_->Draw();
	// 位に応じて数字のスプライトを描画
	for (int i = 0; i < 5; i++) {
		if (eachNumber[i] == 0) {
			spriteNum_[i]->SetTextureHandle(*textureHandles_[1]);
		} else if (eachNumber[i] == 1) {
			spriteNum_[i]->SetTextureHandle(*textureHandles_[2]);
		} else if (eachNumber[i] == 2) {
			spriteNum_[i]->SetTextureHandle(*textureHandles_[3]);
		} else if (eachNumber[i] == 3) {
			spriteNum_[i]->SetTextureHandle(*textureHandles_[4]);
		} else if (eachNumber[i] == 4) {
			spriteNum_[i]->SetTextureHandle(*textureHandles_[5]);
		} else if (eachNumber[i] == 5) {
			spriteNum_[i]->SetTextureHandle(*textureHandles_[6]);
		} else if (eachNumber[i] == 6) {
			spriteNum_[i]->SetTextureHandle(*textureHandles_[7]);
		} else if (eachNumber[i] == 7) {
			spriteNum_[i]->SetTextureHandle(*textureHandles_[8]);
		} else if (eachNumber[i] == 8) {
			spriteNum_[i]->SetTextureHandle(*textureHandles_[9]);
		} else if (eachNumber[i] == 9) {
			spriteNum_[i]->SetTextureHandle(*textureHandles_[10]);
		}
		spriteNum_[i]->Draw();
	}
}

void Score::ScoreReset() {
	sScore_ = 0;
	sKillCount_ = 0;
	
};