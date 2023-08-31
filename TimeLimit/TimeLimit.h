#pragma once
#include "Sprite.h"
#include <memory>
#include <vector>
#include <array>

/// <summary>
/// せいげんｊ
/// </summary>
class TimeLimit {
public:
	// 制限時間
	static const int32_t kTimeLimit = 3;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="textureHandles"></param>
	void Initialize(const std::vector<uint32_t*>& textureHandles);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 経過時間を取得
	/// </summary>
	/// <returns></returns>
	time_t GetElapsedTime() { return elapsedTime_; };

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

private:
	//制限時間のテクスチャハンドル
	std::vector<uint32_t*> textureHandles_{};
	//スプライト
	std::unique_ptr<Sprite> spriteColon_{};
	std::array<std::unique_ptr<Sprite>, 4> spriteNum_{};
	//ゲーム開始時の時間
	bool startFlag_ = false;
	time_t startTime_ = 0;
	//前にフレームの時間
	time_t oldTime_ = 0;
	//現在の時間
	time_t currentTime_ = 0;
	//経過時間
	time_t elapsedTime_ = 0;
	//制限時間
	time_t timeMinutes_ = kTimeLimit;
	time_t timeSeconds_ = 0;
	int32_t eachNumber_[4]{};
};