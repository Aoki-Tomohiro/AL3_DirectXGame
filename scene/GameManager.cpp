#include "GameManager.h"

GameManager::GameManager() { 
	gameTitle_ = new GameTitle(); 
	gameTitle_->Initialize();
	gameScene_ = new GameScene();
	gameScene_->Initialize();
	gameResult_ = new GameResult();
	gameResult_->Initialize();
}

void GameManager::Update() {
	if (currentScene_ == 0) {
		gameTitle_->Update();
	} else if (currentScene_ == 1) {
		gameScene_->Update();
	} else if (currentScene_ == 2) {
		gameResult_->Update();
	}
}

void GameManager::Draw() {
	if (currentScene_ == 0) {
		gameTitle_->Draw();
	} else if (currentScene_ == 1) {
		gameScene_->Draw();
	} else if (currentScene_ == 2) {
		gameResult_->Draw();
	}
}

GameManager::~GameManager() {
	SafeDelete(gameTitle_);
	SafeDelete(gameScene_);
	SafeDelete(gameResult_);
}

void GameManager::ChangeScene(int32_t sceneNum, int32_t oldScene) {
	currentScene_ = sceneNum;

	if (oldScene == 0) {
	
	} else if (oldScene == 1) {
	
	} else if (oldScene == 2) {
	
	}
}