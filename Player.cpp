#include <Player.h>
#include <cassert>
#include "ImGuiManager.h"
#include "MathFunction.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	//シングルインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update() {
	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;
	//回転の速さ
	const float kRotSpeed = 0.02f;

	//押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	//押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN))
	{
		move.y -= kCharacterSpeed;
	}

	//押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

	//キャラクター攻撃処理
	Attack();

	//弾更新
	if (bullet_) {
		bullet_->Update();
	}

	//移動限界座標
	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//座標移動(ベクトルの加算)
	MathFunction::Move(worldTransform_.translation_, move);

	//行列更新
	worldTransform_.matWorld_ = MathFunction::MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//行列を定数バッファに転送
	worldTransform_.TransferMatrix(); 

	//キャラクターの座標を画面表示する処理
	ImGui::Begin("PlayerPosition");
	ImGui::Text(
	    "%f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::End();
}

void Player::Attack() { 
	if (input_->PushKey(DIK_SPACE)){
		//弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_);

		//弾を登録する
		bullet_ = newBullet;
	}
}

void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	if (bullet_) {
		bullet_->Draw(viewProjection);
	}
}