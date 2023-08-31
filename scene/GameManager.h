#pragma once
#include "GameScene.h"
#include "GameTitle.h"
#include "GameResult.h"

class GameManager {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameManager();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// シーン切り替え
	/// </summary>
	/// <param name="newScene"></param>
	void ChangeScene(int32_t sceneNum, int32_t oldScene);

private:
	GameTitle* gameTitle_ = nullptr;
	GameScene* gameScene_ = nullptr;
	GameResult* gameResult_ = nullptr;
	int32_t currentScene_ = 0;
};