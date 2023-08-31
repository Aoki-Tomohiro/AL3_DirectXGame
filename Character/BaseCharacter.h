#pragma once
#include "Audio.h"
#include "Model.h"
#include "WorldTransform.h"
#include <vector>

class BaseCharacter {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models"></param>
	virtual void Initialize(
	    const std::vector<Model*>& models, const std::vector<uint32_t*>& textureHandles,
	    int32_t level);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	virtual void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// ワールド変換データを取得
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_; };

	/// <summary>
	/// デスフラグを取得
	/// </summary>
	/// <returns></returns>
	bool isDead() const { return isDead_; };

protected:
	//モデルデータ配列
	std::vector<Model*> models_;
	//テクスチャハンドル配列
	std::vector<uint32_t*> textureHandles_;
	//ワールド変換データ
	WorldTransform worldTransform_;
	//オーディオ
	Audio* sAudio_ = nullptr;
	//レベル
	int32_t level_;
	////体力
	//int32_t life_;
	//デスフラグ
	bool isDead_ = false;
};