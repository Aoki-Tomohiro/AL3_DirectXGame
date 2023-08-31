#include "BaseCharacter.h"

void BaseCharacter::Initialize(
    const std::vector<Model*>& models, const std::vector<uint32_t*>& textureHandles, int32_t level){ 
	//モデルの初期化
	models_ = models;
	//テクスチャハンドルの取得
	textureHandles_ = textureHandles;
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//オーディオクラスのインスタンスを取得
	sAudio_ = Audio::GetInstance();
	//レベルの初期化
	level_ = level;
}

void BaseCharacter::Update() {
	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void BaseCharacter::Draw(const ViewProjection& viewProjection) {
	//モデル描画
	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection);
	}
}