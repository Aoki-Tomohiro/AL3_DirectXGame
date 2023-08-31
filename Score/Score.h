#pragma once
#include "Sprite.h"
#include <memory>
#include <vector>
#include <array>

/// <summary>
/// スコア
/// </summary>
class Score {
public:
	/// <summary>
	/// 初期化
	/// </summary>
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
	/// スコアのリセット
	/// </summary>
	void ScoreReset();

	/// <summary>
	/// スコアの追加
	/// </summary>
	void AddScore(int32_t score) { sScore_ += score; };

	/// <summary>
	/// 倒した数を増やす
	/// </summary>
	void AddKills() { sKillCount_++; };

	/// <summary>
	/// スコアの取得
	/// </summary>
	/// <returns></returns>
	int32_t GetScore() { return sScore_; };

	/// <summary>
	/// キル数の取得
	/// </summary>
	/// <returns></returns>
	int32_t GetKillCount() { return sKillCount_; };

private:
	//スコアのテクスチャハンドル
	std::vector<uint32_t*> textureHandles_{};
	//スプライト
	std::unique_ptr<Sprite> spriteScore_ = nullptr;
	std::array<std::unique_ptr<Sprite>, 5> spriteNum_{};
	int32_t eachNumber[5]{};
	//スコア
	static int32_t sScore_;
	//倒した数
	static int32_t sKillCount_;
};